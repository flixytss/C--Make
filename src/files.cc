#include "exutils.h"
#include <files.h>
#include <fstream>
#include <ios>
#include <print>
#include <string>

std::string ReadFile(std::string Path) {
    std::ifstream file (Path, std::ios::in);
    if (!file.is_open()) {
        std::println("{}ERR{}: That file dosen't exists", REDB, RESET);
        Finish(1);
    }

    std::string buffer = "";

    std::string line;
    while (std::getline(file, line)) {
        line += '\n';
        buffer.append(line);
    }
    file.close();

    return buffer;
}
void WriteFile(std::string Path, std::string Buffer) {
    std::ofstream file (Path, std::ios::out);
    if (!file.is_open()) {
        std::println("{}ERR{}: That file dosen't exists", REDB, RESET);
        Finish(1);
    }

    file.write(Buffer.c_str(), Buffer.size());

    file.close();
}
void AppendFile(std::string Path, std::string Buffer) {
    std::ofstream file (Path, std::ios::app);
    if (!file.is_open()) {
        std::println("{}ERR{}: That file dosen't exists", REDB, RESET);
        Finish(1);
    }

    file.write(Buffer.c_str(), Buffer.size());

    file.close();
}