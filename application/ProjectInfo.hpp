#pragma once
#include <filesystem>
#include <string>

namespace App
{
struct ProjectInfo
    {
        std::string name;
        std::string description;
        std::string version;
        std::filesystem::path path;
    };
}