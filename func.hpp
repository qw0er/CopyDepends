#pragma once
#include <filesystem>
#include <unordered_set>

struct config
{
    std::filesystem::path exe;
    std::filesystem::path output;
};

config argParse(const int argc,const char *argv[]);

std::unordered_set<std::filesystem::path> exeDepends(const std::filesystem::path& exe);

void copyDepends(const std::unordered_set<std::filesystem::path>& depends, const std::filesystem::path& output, const std::unordered_set<std::
                     string>& excludeList);

std::unordered_set<std::string> dependExcludeList();

std::filesystem::path selfPath();

std::filesystem::path fileRoot();