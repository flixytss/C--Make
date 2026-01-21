#include <algorithm>
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
    bool Havetofree = false;
};

std::tuple<EntryInfo*, bool> GetInf(std::string File) {
    std::vector<std::string> Buffer = GetLines(File);

    std::string ReadMode = "File";
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
                case _str2int("Use Os"):
                    ReadMode = "CopyPreviusOs"; break;
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
                        for (int index = 0; index < 11 /* MODIFY */; index++) { // Modify Every time a new command is created
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
        switch (_str2int(ReadMode)) {
            case _str2int("File"):
                if (Line.empty() || Line.starts_with("#")) continue;
                // Check if the "file" is a directory
                if (std::filesystem::is_directory(l.at(0))) {
                    for (const auto& path : std::filesystem::directory_iterator(l.at(0))) {
                        if (l.size() >= 1 && l.back() != l.front()) {
                            if ((std::string){path.path().filename()}.ends_with(l.at(1)))
                                Inf->Files.push_back(path.path());
                        } else
                            Inf->Files.push_back(path.path());
                        //std::println("{}", (std::string){path.path()});
                    }
                } else
                    Inf->Files.push_back(l.at(0));
                break;
            case _str2int("Largs"):
                if (Line.empty() || Line.starts_with("#")) continue;
                Inf->LinkArgs.push_back(Line); break;
            case _str2int("Cargs"):
                if (Line.empty() || Line.starts_with("#")) continue;
                if (l.back() == l.front()) {
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
                if (l.back() == l.front()) {
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
                        if (l.back() == l.front()) {
                            std::println("{}ERR{}: Set the Output file name, Content \"{}\", Line: {}", REDB, RESET, Line, _Index);
                            Finish(1);
                        }
                        Inf->OutputFile = l.at(1); break;
                    case _str2int("Linker"):
                        if (l.back() == l.front()) {
                            std::println("{}ERR{}: Set Linker argument, Content \"{}\", Line: {}", REDB, RESET, Line, _Index);
                            Finish(1);
                        }
                        Inf->Linker = l.at(1); break;
                    case _str2int("Use"):
                        if (l.back() == l.front()) {
                            std::println("{}ERR{}: Set Os argument, Content \"{}\", Line: {}", REDB, RESET, Line, _Index);
                            Finish(1);
                        }
                        switch (_str2int(l.at(1))) {
                            case _str2int("Linux"):
                                Inf = new EntryInfo { OsEntryInfo.OnlyLinux };
                                OsEntryInfo.Havetofree = true;
                                break;
                            case _str2int("Windows"):
                                Inf = new EntryInfo { OsEntryInfo.OnlyWindows };
                                OsEntryInfo.Havetofree = true;
                                break;
                            case _str2int("Mac"):
                                Inf = new EntryInfo { OsEntryInfo.OnlyMac };
                                OsEntryInfo.Havetofree = true;
                                break;
                            default: break;
                        }
                        
                        break;
                    case _str2int("Cores"):
                        if (l.back() == l.front()) {
                            std::println("{}ERR{}: Set Linker argument, Content \"{}\", Line: {}", REDB, RESET, Line, _Index);
                            Finish(1);
                        }
                        if (l.back() == "All") {
                            Inf->Cores = std::thread::hardware_concurrency(); break;
                        }

                        Inf->Cores = atoi(l.back().c_str()); break;
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

    return std::make_tuple(Inf, OsEntryInfo.Havetofree);
}