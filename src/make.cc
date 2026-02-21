#include <format>
#include <print>
#include <exutils.h>
#include <filesystem>
#include <string>
#include <vector>
#include <files.h>
#include <compilers.h>

// Form the makefile

namespace fs = std::filesystem;

std::string Compilers[4] = {"clang++", "cc", "clang", "nasm"};

class File {
    public:
        std::string path;
        std::string out;
        std::string compiler;
        std::string command;
        std::vector<std::string> args;

        File(std::string _path, std::string _out, std::string _compiler, std::vector<std::string> _args = {}) {
            this->path = _path;
            this->out = _out;
            this->compiler = _compiler;
            this->args = _args;

            this->Recalculate();
        }
        void Recalculate() {
            if (this->out.starts_with('/'))
                this->out.erase(this->out.begin());
            this->command = (std::string) {this->compiler + " -c " + this->path + " -o " + this->out};
            for (std::string a : this->args) this->command += " " + a;
        }
};
typedef struct Function {
    std::string FunctionName = "";
    File InsideFile;
    std::vector<std::string> Utils;
    std::vector<std::string> Dependson;
} Function;
std::vector<Function> Functions;

enum Compilers GetCompiler(std::string filename) {
    if (filename.ends_with(".cc")) return CC;
    if (filename.ends_with(".c++") || filename.ends_with(".cpp")) return CLANGPLUSPLUS;
    if (filename.ends_with(".c")) return CLANG;
    if (filename.ends_with(".s") || filename.ends_with(".asm")) return NASM;
    return CC; // By default
}
void AddArguments(EntryInfo Inf, File* _file) {
    for (const auto& tuple : Inf.CompileArgs) {
        const std::string Arg = std::get<0>(tuple);
        const std::string _Compiler = std::get<1>(tuple);
        if (_Compiler != "All") {
            if (Compilers[GetCompiler(_file->path)] == _Compiler) {
                _file->args.push_back(Arg);
                _file->Recalculate();
            }
        } else {
            _file->args.push_back(Arg);
            _file->Recalculate();
        }
    }
}
std::string SetCompiler(std::vector<enum Compilers> CompilersInUse, enum Compilers l) {         
    int ___idnex = 0;
    std::string ActualCompiler = "$";
    for (const enum Compilers& Com : CompilersInUse) {
        if (Com == l)
            ActualCompiler = "${Var" + std::to_string(___idnex) + '}';
        ___idnex++;
    }
    return ActualCompiler;
}
const std::string tolowercase(const std::string str) {
    for (int i = 0; i < str.size(); i++) if (str.at(i)>=0x41&&str.at(i)<=0x5A) *(char*)&str[i]+=32;
    return str;
}
void MakeFile(EntryInfo* inf) {
    if (!inf) {
        std::println("{}ERR{}: Entry Information not defined", REDB, RESET);
        Finish(1);
    }
    const std::string file = inf->OutputFile.empty() ? "Makefile" : inf->OutputFile;

    std::vector<std::string> Outs;
    std::vector<std::string> OutsForParallel;

    if (inf->CleanUpFirst) {
        File clean {"", "", ""};
        clean.command = inf->CleanUpFirst ? "@echo \"[INFO] Cleaning...\"\n\t-@rm " + (inf->BuildDirectory.empty() ? "*.o" : (inf->BuildDirectory + "/*.o")) : "";

        Function cleanrun {"clean", clean};

        Functions.push_back(cleanrun);
    }

    File run {"", "", ""}; // For the Run command
    run.command = inf->Run.size() ? ("@echo \"[INFO] Running commands...\"\n") : "";

    Function funcrun {"run", run};

    for (const std::string& _run : inf->Run)
        funcrun.Utils.push_back(_run);

    Functions.push_back(funcrun);

    uint Index = 0;
    for (const std::string& filex : inf->Files) {
        for (const auto& tuple : inf->CompilerFilter) {
            if (filex.ends_with(std::get<0>(tuple)))
                Compilers[GetCompiler(filex)] = std::get<1>(tuple);
        }
    }

    std::vector<enum Compilers> CompilersInUse; // Making Variables of compilers
    for (const std::string& filex : inf->Files) {
        const fs::path path = fs::path(filex);
        enum Compilers filex_C = GetCompiler((std::string){path.filename()});

        std::vector<enum Compilers>::iterator it = std::find(CompilersInUse.begin(), CompilersInUse.end(), filex_C);
        if (it == CompilersInUse.end())
            CompilersInUse.push_back(filex_C);
    }

    for (const std::string& file : inf->Files) {
        const fs::path path = fs::path(file);

        File _file {file.c_str(), (std::string){inf->BuildDirectory + "/" + (std::string){path.filename()}} + ".out", (std::string){(inf->Ccache ? "ccache " : "")} + "clang++"};

        for (const auto& tuple : inf->CompilerFilter) {
            if ((std::string){path.filename()}.ends_with(std::get<0>(tuple))) {
                _file = {file.c_str(), (std::string){inf->BuildDirectory + "/" + (std::string){path.filename()}} + ".out", (std::string){(inf->Ccache ? "ccache " : "")} + std::get<1>(tuple)}; break;
            }
        }
        std::string ActualCompiler;
        switch (const enum Compilers Compiler = GetCompiler((std::string){path.filename()})) {
            case CC:
                ActualCompiler = SetCompiler(CompilersInUse, CC);
                _file = {file.c_str(), (std::string){inf->BuildDirectory + "/" + (std::string){path.filename()}} + ".out", (std::string){(inf->Ccache ? "ccache " : "")} + ActualCompiler};
                AddArguments(*inf, &_file);
                break;
            case CLANGPLUSPLUS:
                ActualCompiler = SetCompiler(CompilersInUse, CLANGPLUSPLUS);
                _file = {file.c_str(), (std::string){inf->BuildDirectory + "/" + (std::string){path.filename()}} + ".out", (std::string){(inf->Ccache ? "ccache " : "")} + ActualCompiler};
                AddArguments(*inf, &_file);
                break;
            case CLANG:
                ActualCompiler = SetCompiler(CompilersInUse, CLANG);
                _file = {file.c_str(), (std::string){inf->BuildDirectory + "/" + (std::string){path.filename()}} + ".out", (std::string){(inf->Ccache ? "ccache " : "")} + ActualCompiler};
                AddArguments(*inf, &_file);
                break;
            case NASM:
                ActualCompiler = SetCompiler(CompilersInUse, NASM);
                _file = {file.c_str(), (std::string){inf->BuildDirectory + "/" + (std::string){path.filename()}} + ".out", (std::string){(inf->Ccache ? "ccache " : "")} + ActualCompiler};
                AddArguments(*inf, &_file);
                break;
            default: break; // Use the CLANG syntaxis by default
        }
        
        Outs.push_back((std::string){path.filename()} + ".out");
        OutsForParallel.push_back((std::string){path} + ".out");
        Function func {file + ".out", _file};
        func.Dependson.push_back((std::string){path});
        func.Utils.push_back(std::format("@echo -e \"[\\e[1;32m Compiling file {} {:.1f}%... \\e[0m]\"", _file.path, ((float)Index / (inf->Files.size())) * 100)); // MODIFY
        Functions.push_back(func);

        Index++;
    }

    File f {"", "", ""}; // For the link command
    
    f.command = (inf->Linker.empty() ? Compilers[CompilersInUse.empty() ? 0 : CompilersInUse.at(0)] : inf->Linker);
    
    if (!inf->OnlyLinker) {
        f.command += ' ';
        for (const std::string& out : Outs)
            f.command += inf->BuildDirectory + (inf->BuildDirectory.empty() ?  "" : "/") + out + " ";
    } else {
        for (const std::string& out : inf->Files)
            f.command += inf->BuildDirectory + ' ' + out;
        f.command += ' ';
    }
    const std::string AppPath = inf->BuildDirectory + (inf->BuildDirectory.empty() ? (inf->ProjectName.empty() ? "a.out" : inf->ProjectName) : (inf->ProjectName.empty() ? "/a.out" : "/" + inf->ProjectName));
    f.command += "-o " + AppPath;
    if (inf->LinkArgs.size() > 0)
        for (const std::string& arg : inf->LinkArgs)
            f.command += ' ' + arg;
    

    Function func {"Link", f};
    func.Utils.push_back("@echo -e \"[\\e[1;32m Linking 100%... \\e[0m]\"\n");

    WriteFile(file, (std::string){"# Generated Makefile, Just a template. You can modify me\n"} + (inf->Cores ? "" : "\n")); // Init Makefile
    if (inf->Cores > 0)
        AppendFile(file, "# Why did you enable the cores?, I mean, You just can do \"make -j(Cores)\" \n\n");

    int JustAConuter = 0;
    for (const enum Compilers& Com : CompilersInUse) {
        AppendFile(file, std::format("Var{} = {}\n", JustAConuter, Compilers[Com]));
        JustAConuter++;
    }
    AppendFile(file, "\n");

    // Makefile with Coressss
    if (inf->Cores > 0) {
        const bool EnableCores = tolowercase(inf->OutputFile) == "makefile";

        AppendFile(file, ".DEFAULT_GOAL := parallel");
        AppendFile(file, (std::string){"\nparallel:\n\t${MAKE} "} + (EnableCores ? "" : std::format("-f {} ", inf->OutputFile)) + "-j" + std::to_string(inf->Cores)  + " all\n\tfalse\n"); // Call itselfs with the wanted cores
        func.Utils.push_back("@echo -e \"[\\e[1;32m Parallel build exited correctly, If the makefile says error. Just ignore it... \033[0m]\"\n");
    } else
        AppendFile(file, ".DEFAULT_GOAL := all\n");
    for (const std::string& dep : OutsForParallel)
        func.Dependson.push_back(dep);
    if (inf->OnlyLinker) {
        Functions.clear();
        func.Dependson.clear();
    }
    if (inf->Files.size() || inf->OnlyLinker) Functions.push_back(func);
    

    for (const Function& func : Functions) {
        AppendFile(file, func.FunctionName + (func.Dependson.empty() ? ":\n" : ":"));
        if (!func.Dependson.empty()) {
            for (const std::string& dep : func.Dependson)
                AppendFile(file, ' ' + dep);
            AppendFile(file, "\n");
        }
        for (const std::string& util : func.Utils)
            AppendFile(file, '\t' + util + '\n');

        // making it quiet
        AppendFile(file, (func.InsideFile.command.empty() ? "\t" : "\t@") + func.InsideFile.command + '\n');
    }

    // "All" function
    std::string all = "all:";
    for (const Function& func : Functions) all += "\t" + func.FunctionName;

    AppendFile(file, all + '\n');
    // "Install" function
    const std::string install = AppPath.ends_with(".so") ? std::format("install: all\n\tsudo mv {} /usr/lib", AppPath) : std::format("install:\n\tsudo mv {} /usr/bin", AppPath);
    AppendFile(file, install);

    if (AppPath.ends_with(".so")) {
        std::vector<std::filesystem::directory_entry> headers;
        for (const std::filesystem::directory_entry entry : std::filesystem::recursive_directory_iterator(".")) {
            if (!entry.is_directory()) { if (entry.path().filename().string().ends_with(".h") || entry.path().filename().string().ends_with(".hpp")) if (ReadFile(entry.path()).starts_with('#')) headers.push_back(entry);}
        }
        if (headers.size()) for (const std::filesystem::directory_entry entry : headers) {
            AppendFile(file, std::format("\n\tsudo mv {} /usr/include/{}\n", entry.path().string(), entry.path().filename().string()));
        } else {
            std::println("{}WARN{}: In install function: Only .so files detected, Why would you make a c/c++ library without headers?", YELLOW, RESET);
        }
    }
}