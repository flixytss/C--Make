#include <files.h>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <print>
#include <string>
#include <exutils.h>

EntryInfo inf;

extern void BackgroundProccess(std::string file);

// Complete Verbose Mode // TODOOOOO

constexpr int str2int(std::string str) {
    uint num = 0;
    for (char i : str) num += i;
    return num;
}
int main(int argc, char** argv) {
    if (argc < 1)
        Finish(0);
    EntryInfo* Inf;
    std::string existingfilewithsignature = "";

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
            case str2int("make"):
                if ((std::string){argv[Index + 1] ? argv[Index + 1] : ""} == "template") {
                    std::string cdirectory = ".";

                    if ( argv[Index + 2] ) {
                        cdirectory = argv[Index + 1];
                        std::println("Copying template into {}...", cdirectory);
                    }

                    std::filesystem::create_directory(cdirectory + "/src"); // directories
                    std::filesystem::create_directory(cdirectory + "/build");
                    std::filesystem::create_directory(cdirectory + "/include");

                    WriteFile(cdirectory + "/src/main.cc", "#include <string>\n\nconstexpr long s2i(std::string b){ long l = 0; for (char c : b) l += c; return l; } // AUXILIAR\nint main(int argc, char** argv) { return 0; }");
                    WriteFile(cdirectory + "/compile_flags.txt", "-std=gnu++23\n-Iinclude");

                    WriteFile(cdirectory + "/create.conf", "C++MakeSignature!\n#: File\nsrc/main.cc\n#: Link args\n#: Compiling args\n--std=gnu++23 clang++\n#: Out\nbuild\n#: Project\n\"Default Template\"\n#: Include\ninclude clang++\n#: Info\nUseCcache\n#: Run\n#: Compilers filters\n.cc clang++");
                    break;
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
                Inf = GetInf(existingfilewithsignature.empty() ? (argv[Index + 1] ? argv[Index + 1] : "create.conf") : existingfilewithsignature);
                if (!Inf->Files.size()) {
                    std::println("{}ERR{}: Why would i make a Makefile with no files to compile?", REDB, RESET);
                    Finish(1);
                }
                MakeFile(Inf);
                break;
            default: // For getting the file
                if (Index > 0)
                    if (strcmp(argv[Index - 1], "set-build") != 0 && strcmp(argv[Index - 1], "add-arg") != 0 && strcmp(argv[Index - 1], "make") != 0 && strcmp(argv[Index - 1], "sync") != 0)
                        inf.Files.push_back(Arg);
                break;
        }
    }
    if (inf.Files.size() > 0) MakeFile(Inf);

    Finish(0);
}
