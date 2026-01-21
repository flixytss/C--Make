#pragma once

#include <cstdlib>
#include <format>
#include <print>
#include <string>
#include <tuple>
#include <vector>

#define REDB    "\e[1;31m"
#define GREENB  "\e[1;32m"
#define RED     "\e[0;31m"
#define RESET   "\e[0m"

const std::string HelpMsg = "I don't want to write how to use this again, Please visit \e[1;37mhttps://github.com/flixytss/C--Make\e[0m to see the usage section";

static void Finish(int status) {
    std::println("{}{}{}", status ? std::format("{}ERR{}: Exited with error {}", REDB, RESET, RED) : std::format("{}GOO{}: Exited correctly {}", GREENB, RESET, GREENB), status, RESET); // Complete
    exit(status);
}

typedef struct EntryInfo {
    std::vector<std::string> Files;
    std::string Directory = "";
    std::string BuildDirectory = "";
    std::vector<std::string> LinkArgs;
    std::vector<std::tuple<std::string, std::string>> CompileArgs;
    std::string ProjectName = "";
    bool Ccache = false;
    std::string Compiler = "";
    std::vector<std::string> Run;
    std::vector<std::tuple<std::string, std::string>> CompilerFilter;
    std::string OutputFile = "";
    std::string Linker = "";
    bool CleanUpFirst = false;
    int Cores = 0;
    bool UsingDestructor = false;
} EntryInfo;