#include <filesystem>
#include <fstream>
#include <ios>
#include <sstream>
#include <string>
#include <print>
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
EntryInfo GetInf(std::string File) {
    std::vector<std::string> Buffer = GetLines(File);

    std::string ReadMode = "File";
    EntryInfo Inf;

    int _Index = 0;
    for (const std::string& Line : Buffer) {
        if (Line.starts_with("#: ")) {
            std::string Parameter = getfromindex(Line, 3);
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
                default:
                    if (!Line.starts_with("#")) {
                        std::println("File syntax error");
                        Finish(1);
                    }
            }
        }
        std::vector<std::string> l = split(Line);;
        switch (_str2int(ReadMode)) {
            case _str2int("File"):
                if (Line.starts_with("#")) continue;
                // Check if the "file" is a directory
                if (std::filesystem::is_directory(l[0])) {
                    for (const auto& path : std::filesystem::directory_iterator(l[0])) {
                        if (l.back() != l.front()) {
                            if ((std::string){path.path().filename()}.ends_with(l[1]))
                                Inf.Files.push_back(path.path());
                        } else
                            Inf.Files.push_back(path.path());
                        //std::println("{}", (std::string){path.path()});
                    }
                } else
                    Inf.Files.push_back(l[0]);
                break;
            case _str2int("Largs"):
                if (Line.starts_with("#")) continue;
                Inf.LinkArgs.push_back(Line); break;
            case _str2int("Cargs"):
                if (Line.starts_with("#")) continue;
                if (l.back() == l.front()) {
                    std::println("{}ERR{}: Set for which compilers is the argument!, Content \"{}\", Line: {}", REDB, RESET, Line, _Index);
                    Finish(1);
                }
                Inf.CompileArgs.push_back(std::make_tuple(l[0], l[1])); break;
            case _str2int("Out"):
                if (Line.starts_with("#")) continue;
                Inf.BuildDirectory = Line; break;
            case _str2int("Project"):
                if (Line.starts_with("#")) continue;
                Inf.ProjectName = Line; break;
            case _str2int("Include"):
                if (Line.starts_with("#")) continue;
                if (l.back() == l.front()) {
                    std::println("{}ERR{}: Set for which compilers is the include!, Content \"{}\", Line {}", REDB, RESET, Line, _Index); // FIX
                    Finish(1);
                }
                Inf.CompileArgs.push_back(std::make_tuple("-I " + l[0], l[1])); break;
            case _str2int("Info"):
                if (Line.starts_with("#")) continue;
                switch (_str2int(l[0])) {
                    case _str2int("UseCcache"):
                        Inf.Ccache = true; break;
                    case _str2int("AddClean"):
                        Inf.CleanUpFirst = true; break;
                    case _str2int("OutputFile"):
                        if (l.back() == l.front()) {
                            std::println("{}ERR{}: Set the Output file name, Content \"{}\", Line: {}", REDB, RESET, Line, _Index);
                            Finish(1);
                        }
                        Inf.OutputFile = l[1]; break;
                    case _str2int("Linker"):
                        if (l.back() == l.front()) {
                            std::println("{}ERR{}: Set Linker argument, Content \"{}\", Line: {}", REDB, RESET, Line, _Index);
                            Finish(1);
                        }
                        Inf.Linker = l[1]; break;
                    default:
                        std::println("{}ERR{}: That info dosen't exists, Content: {}", REDB, RESET, l[0]);
                        Finish(1);
                        break;
                } break;
            case _str2int("Compilersfilters"):
                if (Line.starts_with("#")) continue;
                // l[0] = Extension
                // l[1] = Compiler
                Inf.CompilerFilter.push_back(std::make_tuple(l[0], l[1])); break;
            case _str2int("Run"):
                if (Line.starts_with("#")) continue;
                Inf.Run.push_back(Line); break;
            default: break;
        }

        _Index++;
    }

    return Inf;
}