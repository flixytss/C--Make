#include <files.h>
#include <cstring>
#include <filesystem>
#include <print>
#include <string>
#include <exutils.h>

EntryInfo inf;

extern void MakeFile(EntryInfo* inf);
extern EntryInfo* GetInf(std::string File);

constexpr int str2int(std::string str) {
    uint num = 0;
    for (char i : str) num += i;
    return num;
}
int main(int argc, char** argv) {
    if (argc < 1)
        Finish(0);
    EntryInfo* Inf;
    std::string os = "N/A";

    #ifdef __linux
        os = "linux";
    #endif
    #ifdef _WIN32
        os = "win";
    #endif
    #ifdef __APPLE__
        os = "mac";
    #endif

    for (int Index = 0; Index < argc; Index++) {
        const std::string Arg = argv[Index];

        switch (str2int(Arg)) {
            case str2int("help"):
                std::println("{}", HelpMsg);
                break;
            case str2int("--build"):
                if ( !argv[Index + 1] ) {
                    std::println("{}ERR{}: Command Syntax error", REDB, RESET);
                    Finish(1);
                }
                inf.BuildDirectory = argv[Index + 1];
                break;
            case str2int("--arg"):
                if ( !argv[Index + 1] ) {
                    std::println("{}ERR{}: Command Syntax error", REDB, RESET);
                    Finish(1);
                }
                inf.LinkArgs.push_back(argv[Index + 1]);
                break;
            case str2int("make"):
                if ( std::filesystem::exists("mac.conf") && os == "mac" ) {
                    MakeFile(GetInf("mac.conf")); break;
                }
                if ( std::filesystem::exists("win.conf") && os == "win" ) {
                    MakeFile(GetInf("win.conf")); break;
                }
                if ( std::filesystem::exists("linux.conf") && os == "linux" ) {
                    MakeFile(GetInf("linux.conf")); break;
                }
                if ( !std::filesystem::exists("create.conf") ) {
                    WriteFile("create.conf", "#: File\n#: Link args\n#: Compiling args\n#: Out\n#: Project\n#: Include\n#: Info\nUseCcache\n#: Run\n#: Compilers filters"); // FIX
                    Finish(0);
                }
                Inf = GetInf(argv[Index + 1] ? argv[Index + 1] : "create.conf");
                if (!Inf->Files.size()) {
                    std::println("{}ERR{}: Why would i make a Makefile with no files to compile?", REDB, RESET);
                    Finish(1);
                }
                MakeFile(Inf);
                break;
            default: // For getting the file
                if (Index > 0)
                    if (strcmp(argv[Index - 1], "--build") != 0 && strcmp(argv[Index - 1], "--arg") != 0 && strcmp(argv[Index - 1], "make") != 0)
                        inf.Files.push_back(Arg);
                break;
        }
    }
    if (inf.Files.size() > 0) MakeFile(Inf);

    Finish(0);
}