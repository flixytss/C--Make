#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <format>
#include <print>
#include <string>
#include <exutils.h>
#include <vector>
#include <files.h>

std::string FileToCopy = "#include <argumentsea.hpp>\n"
"#include <iostream>\n\n"
"void help(ArgumentsManager* manager) {\n"
"   std::cout << \"Help command! Argument index: \" << manager->get_index() << std::endl;\n"
"}\n"
"void catcher(ArgumentsManager* manager) {\n"
"   std::cout << \"Invalid Argument!: \" << argument << std::endl;\n"
"}\n"
"int main(int argc, char** argv) {\n"
"   ArgumentsManager manager(argv, argc, 1);\n\n"
"   manager.set_catcher(catcher);\n"
"   manager.add(\"help\", help);\n\n"
"   manager.run();\n\n"
"   return 0;\n"
"}";
std::string FileToCopyC23 = "#include <argumentsea.hpp>\n"
"#include <print>\n\n"
"void help(ArgumentsManager* manager) {\n"
"   std::println(\"Help index: {}\", manager->get_index());\n"
"}\n"
"void catcher(ArgumentsManager* manager) {\n"
"   std::println(\"Wrong argument index: {}\", manager->get_index());\n"
"}\n"
"int main(int argc, char** argv) {\n"
"   ArgumentsManager manager(argv, argc, 1);\n\n"
"   manager.set_catcher(catcher);\n"
"   manager.add(\"help\", help);\n\n"
"   manager.run();\n\n"
"   return 0;\n"
"}";

extern void BackgroundProccess(std::string file);
extern void install_library(const std::string path);

const std::string home = getenv("HOME");
const std::string librariesdirectory = home + "/.local/state/c++make/libraries";
std::vector<std::filesystem::directory_entry> libraries;
int RunShellCommand(const std::string c);
const EntryInfo GetInfFromJson(const std::filesystem::path path);

const std::string toupper(const std::string str) {
    for (int i = 0; i < str.size(); i++) if ((str)[i]>=0x61&&(str)[i]<=0x7A) *(char*)&str[i]-=32;
    return str;
}
int main(int argc, char** argv) {
    if (argc < 1)
        Finish(0);
    std::string existingfilewithsignature = "";

    EntryInfo* inf_ptr;
    if ( !std::filesystem::exists(home + "/.local/state/c++make/libraries") ) std::filesystem::create_directories(home + "/.local/state/c++make/libraries");

    for (const auto path : std::filesystem::directory_iterator(librariesdirectory))
        if (path.is_directory()) libraries.push_back(path);

    const auto isin = [](const std::filesystem::path entry) {
        bool in = false;
        for (const std::filesystem::directory_entry ent : libraries) {
            if (ent.path().string() == entry.string()) in = true;
        }
        return in;
    };

    const auto compilefile = [](const std::string path, const std::string compiler){
        int errco;
        std::println("{}\t{}", toupper(compiler), path);
        if ( (errco = RunShellCommand(std::format("{} {} -o {}", compiler, path, std::filesystem::path(path).filename().string()))) < 0) {
            std::println("{}ERR{}: Compiler failed with error code: {}", REDB, RESET, errco);
            Finish(1);
        }
    };

    for (int Index = 0; Index < argc; Index++) {
        const std::string Arg = argv[Index];

        switch (str2int(Arg)) {
            case str2int("help"):
                std::println("{}", HelpMsg);
                break;
            case str2int("sync"):
                if ( !argv[Index + 1] ) {
                    std::println("{}ERR{}: Command Syntax error", REDB, RESET);
                    Finish(1);
                }
                BackgroundProccess(argv[Index + 1]);
                Finish(0);
            case str2int("install"):
                if ( !argv[Index + 1] ) {
                    std::println("{}ERR{}: Command Syntax error", REDB, RESET);
                    Finish(1);
                }
                if ( !std::filesystem::exists(argv[Index + 1]) ) {
                    std::println("{}ERR{}: That directory dosen't exists", REDB, RESET);
                    Finish(1);
                }
                std::println("{}INF{}: Installing library {}...", YELLOW, RESET, std::filesystem::path(argv[Index + 1]).filename().string());
                install_library(argv[Index + 1]);
                std::println("{}INF{}: Library installed locally!", YELLOW, RESET);
                Finish(0);
            case str2int("remove"):
                if ( !argv[Index + 1] ) {
                    std::println("{}ERR{}: Command Syntax error", REDB, RESET);
                    Finish(1);
                }
                if ( !isin(std::filesystem::path(librariesdirectory + "/" + argv[Index + 1])) ) {
                    std::println("{}ERR{}: That library dosen't exists", REDB, RESET);
                    Finish(1);
                }
                std::println("{}INF{}: Removing library {}...", YELLOW, RESET, argv[Index + 1]);
                std::filesystem::remove_all(librariesdirectory + "/" + argv[Index + 1]);
                std::println("{}INF{}: Library removed!", YELLOW, RESET);
                Finish(0);
            case str2int("bin"):
                if ( !argv[Index + 1] || !argv[Index + 2] ) {
                    std::println("{}ERR{}: Command Syntax error", REDB, RESET);
                    Finish(1);
                }
                if ( !isin(std::filesystem::path(librariesdirectory + "/" + argv[Index + 1])) ) {
                    std::println("{}ERR{}: That library dosen't exists", REDB, RESET);
                    Finish(1);
                }
                std::system((std::string){librariesdirectory + "/" + argv[Index + 1] + "/bin/" + argv[Index + 2]}.c_str());
                Finish(0);
            case str2int("libraries"):
                std::println("Installed libraries:");
                for (auto path : libraries) { std::println("\t{}: {}", path.path().filename().string(), path.path().string()); }
                break;
            case str2int("json"):
                if ( !argv[Index + 1] ) {
                    std::println("{}ERR{}: Command Syntax error", REDB, RESET);
                    Finish(1);
                }
                inf_ptr = new EntryInfo;
                *inf_ptr = GetInfFromJson(std::filesystem::path(argv[Index + 1]));
                MakeFile(inf_ptr);
                delete inf_ptr;

                Finish(0);
            case str2int("make"):
                if ((std::string){argv[Index + 1] ? argv[Index + 1] : ""} == "template") {
                    std::string cdirectory = ".";

                    if ( argv[Index + 2] ) {
                        cdirectory = argv[Index + 2];
                        std::println("Copying template into {}...", cdirectory);
                    }
                    if ( !std::filesystem::exists(cdirectory) && cdirectory != "." ) std::filesystem::create_directories(cdirectory);

                    std::filesystem::create_directory(cdirectory + "/src"); // directories
                    std::filesystem::create_directory(cdirectory + "/build");
                    std::filesystem::create_directory(cdirectory + "/include");

                    #ifndef ARGUMENTSEA
                        WriteFile(cdirectory + "/src/main.cpp", "#include <string>\n\nconstexpr long s2i(std::string b){ long l = 0; for (char c : b) l += c; return l; } // AUXILIAR\nint main(int argc, char** argv) { return 0; }");
                        WriteFile(cdirectory + "/create.conf", "C++MakeSignature!\n#: File\nsrc .cpp\n#: Link args\n#: Compiling args\n--std=gnu++23 clang++\n#: Out\nbuild\n#: Project\n\"Default Template\"\n#: Include\ninclude clang++\n#: Info\nUseCcache\n#: Run\n#: Compilers filters\n.cpp clang++");
                    #else
                        #ifndef CPP23
                            WriteFile(cdirectory + "/src/main.cpp", FileToCopy);
                        #else
                            WriteFile(cdirectory + "/src/main.cpp", FileToCopyC23);
                        #endif
                        WriteFile(cdirectory + "/create.conf", "C++MakeSignature!\n#: File\nsrc .cpp\n#: Link args\n-largumentsea\n#: Compiling args\n--std=gnu++23 clang++\n#: Out\nbuild\n#: Project\n\"Default Template\"\n#: Include\ninclude clang++\n#: Info\nUseCcache\n#: Run\n#: Compilers filters\n.cpp clang++");
                    #endif
                    WriteFile(cdirectory + "/compile_flags.txt", "-std=gnu++23\n-Iinclude");

                    Finish(0);
                }

                if ( !std::filesystem::exists("create.conf") ) {
                    for (const auto& path : std::filesystem::directory_iterator(std::filesystem::current_path())) {
                        if (ReadFile(path.path()).starts_with("C++MakeSignature!"))
                            existingfilewithsignature = path.path();
                    }
                    if (existingfilewithsignature.empty()) {
                        WriteFile("create.conf", "C++MakeSignature!\n#: File\n#: Link args\n#: Compiling args\n#: Out\n#: Project\n#: Include\n#: Info\nUseCcache\n#: Run\n#: Compilers filters");
                        Finish(0);
                    }
                }
                inf_ptr = GetInf(existingfilewithsignature.empty() ? (argv[Index + 1] ? argv[Index + 1] : "create.conf") : existingfilewithsignature);
                if (!inf_ptr->Files.size()) {
                    std::println("{}ERR{}: Why would i make a Makefile with no files to compile?", REDB, RESET);
                    Finish(1);
                }
                MakeFile(inf_ptr);
                Finish(0);
            default:
                if (!(Index > 0)) break;
                if ( !std::filesystem::exists(Arg) )
                    std::println("{}ERR{}: That command (or file) dosen't exists", REDB, RESET);
                else {
                    if (Arg.ends_with(".c")) { compilefile(Arg, "clang"); }
                    if (Arg.ends_with(".cc")) { compilefile(Arg, "cc"); }
                    if (Arg.ends_with(".cpp")) { compilefile(Arg, "clang++"); }
                }
                Finish(1);
                break;
        }
    }

    Finish(0);
}
int RunShellCommand(const std::string c) {
    FILE* pipe = popen(c.c_str(), "r");

    char* buffer = new char[1024];
    while (fgets(buffer, 1024, pipe) != NULL) std::print("{}", buffer);

    return WEXITSTATUS(pclose(pipe));
}