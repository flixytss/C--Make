#pragma once

#include <cstdlib>
#include <filesystem>
#include <format>
#include <print>
#include <string>
#include <sys/types.h>
#include <tuple>
#include <vector>

#define REDB    "\e[1;31m"
#define GREENB  "\e[1;32m"
#define RED     "\e[0;31m"
#define YELLOW  "\e[0;33m"
#define RESET   "\e[0m"

const std::string HelpMsg = "I don't want to write how to use this again, Please visit \e[1;37mhttps://github.com/flixytss/C--Make\e[0m to see the usage section";
extern const std::string home;
extern const std::string librariesdirectory;
extern std::vector<std::filesystem::directory_entry> libraries;

constexpr int str2int(std::string str) {
    uint num = 0;
    for (char i : str) num += i;
    return num;
}

static void Finish(int status) {
    std::println("{}{}{}", status ? std::format("{}ERR{}: Exited with error {}", REDB, RESET, RED) : std::format("{}GOO{}: Exited correctly {}", GREENB, RESET, GREENB), status, RESET); // Complete
    exit(status);
}

typedef struct EntryInfo {
    std::string ProjectName = "";
    std::string OutputFile = "";
    std::string Linker = "";
    std::string BuildDirectory = "";
    std::vector<std::string> LinkArgs;
    std::vector<std::string> Run;
    std::vector<std::string> Files;
    std::vector<std::tuple<std::string, std::string>> CompileArgs;
    std::vector<std::tuple<std::string, std::string>> CompilerFilter;
    std::vector<std::tuple<std::string, std::string>> Launchers;
    bool CleanUpFirst = false;
    bool Ccache = false;
    bool OnlyLinker = false;
    int Cores = 0;
} EntryInfo;

extern void MakeFile(EntryInfo*);
extern EntryInfo* GetInf(std::string File, bool verbose = false);
extern std::vector<std::string> GetLines(std::string File);