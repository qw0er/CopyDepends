#pragma once
#include <filesystem>
#include <vector>

struct config
{
    std::filesystem::path exe;
    std::filesystem::path output;
};

config argParse(const int argc,const char *argv[]);

std::vector<std::filesystem::path> exeDepends(const std::filesystem::path& exe);

void copyDepends(const std::vector<std::filesystem::path> &depends, const std::filesystem::path& output);