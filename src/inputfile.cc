#include <algorithm>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <ios>
#include <sstream>
#include <string>
#include <print>
#include <thread>
#include <tuple>
#include <vector>
#include <exutils.h>

std::vector<std::string> GetLines(std::string File) {
    std::ifstream file (File, std::ios::in);
    if (!file.is_open())
        Finish(1);

    std::vector<std::string> Buffer;

    std::string line;
    while (std::getline(file, line))
        Buffer.push_back(line);

    file.close();

    return Buffer;
}
int findindex(std::string string, char c) {
    uint index = 0;
    while ( string[index] != c ) {
        if (index != string.size())
            index++;
        else {
            index = 0;
            break;
        }
    }
    return index;
}
bool IsItVerySimilar(std::string what, std::string to, int wantedlevel) {
    int lookedlike = 0;
    int limit = std::min(what.size(), to.size());

    for (int i = 0;i < limit; i++)
        if (to[i] == what[i]) lookedlike++;
    return lookedlike >= wantedlevel;
}
std::string getfromindex(std::string str, int index) {
    std::string string = "";
    for (int i = index;i < str.size(); i++)
        string += str[i];
    return string;
}
std::vector<std::string> split(std::string string, char delimeter = ' ') {
    std::stringstream ss(string);
    std::string line;
    std::vector<std::string> tokens;

    while (getline(ss, line, delimeter)) tokens.push_back(line);
    return tokens;
}

// Tu Tu ru: Bro is not okarin

constexpr int _str2int(std::string str) {
    uint num = 0;
    for (char i : str) num += i;
    return num;
}

const std::string CommandsForSyntaxisFallback[11] = {"File", "Link args", "Out", "Project", "Include", "Info", "Run", "Compilers filters", "Windows", "Mac", "Linux"};

struct DifferentsOsEntryInfo {
    EntryInfo OnlyWindows {};
    EntryInfo OnlyLinux {};
    EntryInfo OnlyMac {};
};

int files_inside_a_directory(const std::string path) {
    int files_or_directory = 0;
    for (const auto _ : std::filesystem::directory_iterator(path)) files_or_directory++;
    return files_or_directory;
}

EntryInfo* GetInf(std::string File, bool verbose) {
    std::vector<std::string> Buffer = GetLines(File);

    std::string ReadMode = "TextMode";
    EntryInfo* Inf;
    bool osModeEnabled = false;
    static DifferentsOsEntryInfo OsEntryInfo {};

    int _Index = 0;
    for (const std::string& Line : Buffer) {
        if (Line.starts_with("#: ")) {
            std::string Parameter = Line.empty() ? "" : getfromindex(Line, 3);
            switch (_str2int(Parameter)) {
                case _str2int("File"):
                    ReadMode = "File"; break;
                case _str2int("Link args"):
                    ReadMode = "Largs"; break;
                case _str2int("Compiling args"):
                    ReadMode = "Cargs"; break;
                case _str2int("Out"):
                    ReadMode = "Out"; break;
                case _str2int("Project"):
                    ReadMode = "Project"; break;
                case _str2int("Include"):
                    ReadMode = "Include"; break;
                case _str2int("Info"):
                    ReadMode = "Info"; break;
                case _str2int("Run"):
                    ReadMode = "Run"; break;
                case _str2int("Compilers filters"):
                    ReadMode = "Compilersfilters"; break;
                case _str2int("Windows"):
                    osModeEnabled = true;
                    ReadMode = "Windows"; break;
                case _str2int("Linux"):
                    osModeEnabled = true;
                    ReadMode = "Linux"; break;
                case _str2int("Mac"):
                    osModeEnabled = true;
                    ReadMode = "Mac"; break;
                default:
                    if (Line.starts_with("#")) {
                        for (int index = 0; index < 11 /* MODIFY */; index++) { // Modify Every time a new section is created
                            if (IsItVerySimilar(Parameter, CommandsForSyntaxisFallback[index], CommandsForSyntaxisFallback[index].size() / 2)) {
                                std::println("{}ERR{}: Don't you mean {}?, Line: {}", REDB, RESET, CommandsForSyntaxisFallback[index], Line);
                                Finish(1);
                            }
                        }
                        std::println("{}ERR{}: Syntax error, Line: {}", REDB, RESET, Line);
                        Finish(1);
                    } break;
            }
        }
        if (osModeEnabled) {
            if (ReadMode == "Windows") Inf = &OsEntryInfo.OnlyWindows;
            if (ReadMode == "Linux") Inf = &OsEntryInfo.OnlyLinux;
            if (ReadMode == "Mac") Inf = &OsEntryInfo.OnlyMac;
        } else {
            #ifdef __linux
                Inf = &OsEntryInfo.OnlyLinux;
            #endif
            #ifdef _WIN32
                Inf = &OsEntryInfo.OnlyWindows;
            #endif
            #ifdef __APPLE__
                Inf = &OsEntryInfo.OnlyMac;
            #endif
        }

        std::vector<std::string> l = split(Line);
        std::string* library = NULL;
        switch (_str2int(ReadMode)) {
            case _str2int("File"):
                if (Line.empty() || Line.starts_with("#")) continue;
                // Check if the "file" is a directory
                if (std::filesystem::is_directory(l.at(0))) {
                    for (const auto& path : std::filesystem::directory_iterator(l.at(0))) {
                        if (l.size() > 1) {
                            if ((std::string){path.path().filename()}.ends_with(l.at(1)))
                                Inf->Files.push_back(path.path());
                        } else
                            Inf->Files.push_back(path.path());
                    }
                } else {
                    Inf->Files.push_back(l.at(0));
                }
                break;
            case _str2int("Largs"):
                if (Line.empty() || Line.starts_with("#")) continue;
                Inf->LinkArgs.push_back(Line); break;
            case _str2int("Cargs"):
                if (Line.empty() || Line.starts_with("#")) continue;
                if (!(l.size() > 1)) {
                    std::println("{}ERR{}: Set for which compilers is the argument!, Content \"{}\", Line: {}", REDB, RESET, Line, _Index);
                    Finish(1);
                }
                Inf->CompileArgs.push_back(std::make_tuple(l.at(0), l.at(1))); break;
            case _str2int("Out"):
                if (Line.empty() || Line.starts_with("#")) continue;
                Inf->BuildDirectory = Line; break;
            case _str2int("Project"):
                if (Line.empty() || Line.starts_with("#")) continue;
                Inf->ProjectName = Line; break;
            case _str2int("Include"):
                if (Line.empty() || Line.starts_with("#")) continue;
                if (!(l.size() > 1)) {
                    std::println("{}ERR{}: Set for which compilers is the include!, Content \"{}\", Line {}", REDB, RESET, Line, _Index);
                    Finish(1);
                }
                Inf->CompileArgs.push_back(std::make_tuple("-I " + l.at(0), l.at(1))); break;
            case _str2int("Info"):
                if (Line.empty() || Line.starts_with("#")) continue;
                switch (_str2int(l.at(0))) {
                    case _str2int("UseCcache"):
                        Inf->Ccache = true; break;
                    case _str2int("AddClean"):
                        Inf->CleanUpFirst = true; break;
                    case _str2int("OutputFile"):
                        if (!(l.size() > 1)) {
                            std::println("{}ERR{}: Set the Output file name, Content \"{}\", Line: {}", REDB, RESET, Line, _Index);
                            Finish(1);
                        }
                        Inf->OutputFile = l.at(1); break;
                    case _str2int("Linker"):
                        if (!(l.size() > 1)) {
                            std::println("{}ERR{}: Set Linker argument, Content \"{}\", Line: {}", REDB, RESET, Line, _Index);
                            Finish(1);
                        }
                        Inf->Linker = l.at(1); break;
                    case _str2int("Use"):
                        if (!(l.size() > 1)) {
                            std::println("{}ERR{}: Set Os argument, Content \"{}\", Line: {}", REDB, RESET, Line, _Index);
                            Finish(1);
                        }
                        switch (_str2int(l.at(1))) {
                            case _str2int("Linux"):
                                *Inf = { OsEntryInfo.OnlyLinux }; break;
                            case _str2int("Windows"):
                                *Inf = { OsEntryInfo.OnlyWindows }; break;
                            case _str2int("Mac"):
                                *Inf = { OsEntryInfo.OnlyMac }; break;
                            default: break;
                        }
                        
                        break;
                    case _str2int("UseLib"):
                        if (!(l.size() > 2)) {
                            std::println("{}ERR{}: Set lib argument, Content \"{}\", Line: {}", REDB, RESET, Line, _Index);
                            Finish(1);
                        }
                        
                        // finding library and checking it
                        for (const auto li : libraries) if (l.at(1) == li.path().filename().string()) {
                            if (!std::filesystem::exists(li.path().string() + "/include") || !std::filesystem::exists(li.path().string() + "/lib")) {
                                std::println("{}ERR{}: Library {} dosen't have the correct structure, Content \"{}\", Line: {}", REDB, RESET, li.path().string(), Line, _Index);
                                Finish(1);
                            }

                            library = new std::string;
                            library->resize(li.path().string().size());
                            memcpy(library->data(), li.path().c_str(), li.path().string().size());

                            if (!files_inside_a_directory(li.path().string() + "/include")) {
                                std::println("{}WARN{}: Using empty library {}", YELLOW, RESET, l.at(1));
                            }
                        }
                        if (!library) {
                            std::println("{}ERR{}: Library not found! (Try installing it in ~/.local/state/c++make/libraries/\"Your library\"), Content \"{}\", Line: {}", REDB, RESET, Line, _Index);
                            Finish(1);
                        }
                        std::println("{}INF{}: Using library {}", YELLOW, RESET, l.at(1));

                        Inf->CompileArgs.push_back(std::make_tuple(("-I" + (*library) + "/include"), l.back()));
                        Inf->LinkArgs.push_back(("-L" + (*library) + "/lib"));

                        delete library;
                        library = NULL;
                        
                        break;
                    case _str2int("Cores"):
                        if (!(l.size() > 1)) {
                            std::println("{}ERR{}: Set Linker argument, Content \"{}\", Line: {}", REDB, RESET, Line, _Index);
                            Finish(1);
                        }
                        if (l.back() == "All") {
                            if (verbose) std::println("[DEBUG] Cores found: {}", std::thread::hardware_concurrency());
                            Inf->Cores = std::thread::hardware_concurrency(); break;
                        }
                        Inf->Cores = atoi(l.back().c_str()); break;
                    case _str2int("OnlyUseLinker"):
                        Inf->OnlyLinker = true; break;
                    case _str2int("UseLauncher"):
                        if (!(l.size() > 2)) {
                            std::println("{}ERR{}: Set Launcher argument, Content \"{}\", Line: {}", REDB, RESET, Line, _Index);
                            Finish(1);
                        }
                        Inf->Launchers.push_back(std::make_tuple(l.at(1), l.at(2))); break;
                    default:
                        std::println("{}ERR{}: That info dosen't exists, Content: {}", REDB, RESET, l.at(0));
                        Finish(1);
                } break;
            case _str2int("Compilersfilters"):
                if (Line.empty() || Line.starts_with("#")) continue;
                if (l.empty()) continue;
                if (!l.empty()) Inf->CompilerFilter.push_back(std::make_tuple(l.at(0), l.at(1)));
                break;
            case _str2int("Run"):
                if (Line.empty() || Line.starts_with("#")) continue;
                Inf->Run.push_back(Line); break;
            default: break;
        }

        _Index++;
    }
    #ifdef __linux
        Inf = &OsEntryInfo.OnlyLinux;
    #endif
    #ifdef __WIN32
        Inf = &OsEntryInfo.OnlyWindows;
    #endif
    #ifdef __APPLE__
        Inf = &OsEntryInfo.OnlyMac;
    #endif

    return Inf;
}