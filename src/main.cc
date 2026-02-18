#include <cstdlib>
#include <cstring>
#include <files.h>
#include <filesystem>
#include <print>
#include <string>
#include <exutils.h>
#include <vector>

std::string FileToCopy = "#include <argumentsea.hpp>\n"
"#include <iostream>\n"
"#include <string>\n\n"
"int main(int argc, char** argv) {\n"
"    ArgumentsManager manager(argv, argc, 1);\n\n"
"    manager.set_catcher([](const std::string argument){\n"
"        std::cout << \"Invalid Argument!: \" << argument << std::endl;\n"
"    });\n\n"
"    manager.add(\"help\", [](const int index){\n"
"        std::cout << \"Help command! Argument index: \" << index << std::endl;\n"
"    });\n\n"
"    manager.run();\n\n"
"    return 0;\n"
"}";

extern void BackgroundProccess(std::string file);
extern void install_library(const std::string path);

const std::string home = getenv("HOME");
const std::string librariesdirectory = home + "/.local/state/c++make/libraries";
std::vector<std::filesystem::directory_entry> libraries;

constexpr int str2int(std::string str) {
    uint num = 0;
    for (char i : str) num += i;
    return num;
}
int main(int argc, char** argv) {
    if (argc < 1)
        Finish(0);
    std::string existingfilewithsignature = "";

    EntryInfo inf;
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

    for (int Index = 0; Index < argc; Index++) {
        const std::string Arg = argv[Index];

        switch (str2int(Arg)) {
            case str2int("help"):
                std::println("{}", HelpMsg);
                break;
            case str2int("set-build"):
                if ( !argv[Index + 1] ) {
                    std::println("{}ERR{}: Command Syntax error", REDB, RESET);
                    Finish(1);
                }
                inf.BuildDirectory = argv[Index + 1];
                break;
            case str2int("add-arg"):
                if ( !argv[Index + 1] ) {
                    std::println("{}ERR{}: Command Syntax error", REDB, RESET);
                    Finish(1);
                }
                inf.LinkArgs.push_back(argv[Index + 1]);
                break;
            case str2int("sync"):
                if ( !argv[Index + 1] ) {
                    std::println("{}ERR{}: Command Syntax error", REDB, RESET);
                    Finish(1);
                }
                BackgroundProccess(argv[Index + 1]);
                break;
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
                break;
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
                break;
            case str2int("libraries"):
                std::println("Installed libraries:");
                for (auto path : libraries) { std::println("\t{}: {}", path.path().filename().string(), path.path().string()); }
                break;
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
                        WriteFile(cdirectory + "/src/main.cc", "#include <string>\n\nconstexpr long s2i(std::string b){ long l = 0; for (char c : b) l += c; return l; } // AUXILIAR\nint main(int argc, char** argv) { return 0; }");
                        WriteFile(cdirectory + "/create.conf", "C++MakeSignature!\n#: File\nsrc/main.cc\n#: Link args\n#: Compiling args\n--std=gnu++23 clang++\n#: Out\nbuild\n#: Project\n\"Default Template\"\n#: Include\ninclude clang++\n#: Info\nUseCcache\n#: Run\n#: Compilers filters\n.cc clang++");
                    #else
                        WriteFile(cdirectory + "/src/main.cc", FileToCopy);
                        WriteFile(cdirectory + "/create.conf", "C++MakeSignature!\n#: File\nsrc/main.cc\n#: Link args\n-largumentsea\n#: Compiling args\n--std=gnu++23 clang++\n#: Out\nbuild\n#: Project\n\"Default Template\"\n#: Include\ninclude clang++\n#: Info\nUseCcache\n#: Run\n#: Compilers filters\n.cc clang++");
                    #endif
                    WriteFile(cdirectory + "/compile_flags.txt", "-std=gnu++23\n-Iinclude");

                    return 0;
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
                break;
            default: // For getting the file
                if (Index > 0)
                    if (strcmp(argv[Index - 1], "set-build") != 0 && strcmp(argv[Index - 1], "add-arg") != 0 && strcmp(argv[Index - 1], "make") != 0 && strcmp(argv[Index - 1], "sync") != 0)
                        inf.Files.push_back(Arg);
                break;
        }
    }
    if (inf.Files.size() > 0) MakeFile(&inf);

    Finish(0);
}
