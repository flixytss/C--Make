#include <algorithm>
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

            if (this->out.starts_with('/'))
                this->out.erase(this->out.begin() + 0);
            this->command = (std::string) {this->compiler + " -c " + this->path + " -o " + this->out + ' '}; // Set more compilers syntax
            for (std::string a : this->args) this->command += a + " ";
        }
        void Recalculate() {
            if (this->out.starts_with('/'))
                this->out.erase(this->out.begin() + 0);
            this->command = (std::string) {this->compiler + " -c " + this->path + " -o " + this->out + ' '}; // Set more compilers syntax
            for (std::string a : this->args) this->command += a + " ";
        }
};
typedef struct Function {
    std::string FunctionName = "";
    File InsideFile;
    std::vector<std::string> Utils;
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
            std::println("{} {}", _Compiler, Inf.Compiler == _Compiler);
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
void SetCompiler(std::vector<enum Compilers> CompilersInUse, enum Compilers l, std::string* ActualCompiler) {         
    int ___idnex = 0;
    for (const enum Compilers& Com : CompilersInUse) {
        if (Com == l)
            *ActualCompiler = "${Var" + std::to_string(___idnex) + '}';
        ___idnex++;
    }
}
void MakeFile(EntryInfo inf) {
    const std::string file = "Makefile";
    std::vector<std::string> Outs;

    File run {"", "", ""}; // For the Run command
    run.command = inf.Run.size() ? "echo \"[INFO] Running commands...\"\n" : "";

    Function funcrun {"Run", run};

    for (const std::string& _run : inf.Run)
        funcrun.Utils.push_back(_run);

    Functions.push_back(funcrun);

    uint Index = 0;
    enum Compilers _Compiler;
    for (const std::string& filex : inf.Files) {
        for (const auto& tuple : inf.CompilerFilter) {
            if (filex.ends_with(std::get<0>(tuple)))
                Compilers[GetCompiler(filex)] = std::get<1>(tuple);
        }
    }

    std::vector<enum Compilers> CompilersInUse; // Making Variables of compilers
    for (const std::string& filex : inf.Files) {
        const fs::path path = fs::u8path(filex);
        enum Compilers filex_C = GetCompiler((std::string){path.filename()});

        std::vector<enum Compilers>::iterator it = std::find(CompilersInUse.begin(), CompilersInUse.end(), filex_C);
        if (it == CompilersInUse.end())
            CompilersInUse.push_back(filex_C);
    } // CHECK IF ERROR

    for (const std::string& file : inf.Files) {
        const fs::path path = fs::u8path(file);

        // Define file extension
        const enum Compilers Compiler = GetCompiler((std::string){path.filename()});
        _Compiler = Compiler;

        File _file {file.c_str(), (std::string){inf.BuildDirectory + "/" + (std::string){path.filename()}} + ".o", (std::string){(inf.Ccache ? "ccache " : "")} + "clang++"};

        int ___idnex = 0; // Just for the loop, That's why it has that name
        for (const auto& tuple : inf.CompilerFilter) {
            if ((std::string){path.filename()}.ends_with(std::get<0>(tuple))) {
                _file = {file.c_str(), (std::string){inf.BuildDirectory + "/" + (std::string){path.filename()}} + ".o", (std::string){(inf.Ccache ? "ccache " : "")} + std::get<1>(tuple)}; break;
            }
        }
        std::string ActualCompiler;
        switch (Compiler) {
            case CC:
                ActualCompiler = "$";
                SetCompiler(CompilersInUse, CC, &ActualCompiler);
                _file = {file.c_str(), (std::string){inf.BuildDirectory + "/" + (std::string){path.filename()}} + ".o", (std::string){(inf.Ccache ? "ccache " : "")} + ActualCompiler};
                AddArguments(inf, &_file);
                break;
            case CLANGPLUSPLUS:
                ActualCompiler = "$";
                SetCompiler(CompilersInUse, CLANGPLUSPLUS, &ActualCompiler);
                _file = {file.c_str(), (std::string){inf.BuildDirectory + "/" + (std::string){path.filename()}} + ".o", (std::string){(inf.Ccache ? "ccache " : "")} + ActualCompiler};
                AddArguments(inf, &_file);
                break;
            case CLANG:
                ActualCompiler = "$";
                SetCompiler(CompilersInUse, CC, &ActualCompiler);
                _file = {file.c_str(), (std::string){inf.BuildDirectory + "/" + (std::string){path.filename()}} + ".o", (std::string){(inf.Ccache ? "ccache " : "")} + ActualCompiler};
                AddArguments(inf, &_file);
                break;
            case NASM:
                ActualCompiler = "$";
                SetCompiler(CompilersInUse, NASM, &ActualCompiler);
                _file = {file.c_str(), (std::string){inf.BuildDirectory + "/" + (std::string){path.filename()}} + ".o", (std::string){(inf.Ccache ? "ccache " : "")} + ActualCompiler};
                AddArguments(inf, &_file);
                break;
            default: break; // Use the CLANG syntaxis by default
        }
        
        Outs.push_back((std::string){path.filename()} + ".o");
        Function func {file + ".o", _file};
        func.Utils.push_back(std::format("@echo \"[{} Compiling file {} {:.1f}%...{}]\"", GREENB, _file.path, ((float)Index / (inf.Files.size())) * 100, RESET)); // MODIFY
        Functions.push_back(func);

        Index++;
    }

    File f {"", "", ""}; // For the link command
    
    if (inf.Compiler.empty())
        f.command = Compilers[_Compiler] + " ";
    else
        f.command = inf.Compiler + " ";
    for (const std::string& out : Outs)
        f.command += inf.BuildDirectory + (inf.BuildDirectory.empty() ?  "" : "/") + out + " ";
    const std::string AppPath = inf.BuildDirectory + (inf.BuildDirectory.empty() ? (inf.ProjectName.empty() ? "a.out" : inf.ProjectName) : (inf.ProjectName.empty() ? "/a.out" : "/" + inf.ProjectName));
    f.command += "-o " + AppPath;
    if (inf.LinkArgs.size() > 0)
        for (const std::string& arg : inf.LinkArgs)
            f.command += ' ' + arg;

    Function func {"Link", f};
    func.Utils.push_back(std::format("@echo \"[{} Linking 100%...{}]\"\n", GREENB,  RESET));
    Functions.push_back(func);

    WriteFile(file, "# Generated Makefile, Just a template. You can modify me\n\n"); // Init Makefile

    int ___idnex = 0; // Just for the loop, That's why it has that name
    for (const enum Compilers& Com : CompilersInUse) {
        AppendFile(file, std::format("Var{} = {}\n", ___idnex, Compilers[Com]));
        ___idnex++;
    }
    AppendFile(file, "\n");

    for (const Function& func : Functions) {
        AppendFile(file, func.FunctionName + ":\n");
        AppendFile(file, '\t' + func.InsideFile.command + '\n');
        for (const std::string& util : func.Utils)
            AppendFile(file, '\t' + util + '\n');
    }
    //
    // "All" function
    //
    std::string all = "all:";
    for (const Function& func : Functions) all += "\t" + func.FunctionName;

    AppendFile(file, all + '\n');
    //
    // "Install" function
    //
    const std::string install = std::format("install:\n\tsudo mv {} /usr/bin", AppPath);
    AppendFile(file, install);
}