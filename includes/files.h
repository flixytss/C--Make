#pragma once

#include <string>

std::string ReadFile(std::string Path);
void WriteFile(std::string Path, std::string Buffer);
void AppendFile(std::string Path, std::string Buffer);