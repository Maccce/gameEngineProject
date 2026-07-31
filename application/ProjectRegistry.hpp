#pragma once
#include "ProjectInfo.hpp"
#include <cstddef>
#include <filesystem>
#include <vector>

namespace App
{
class ProjectRegistry
    {
    public:
        void discover(const std::filesystem::path& projectsDirectory);
        const std::vector<ProjectInfo>& available() const;
        const ProjectInfo& project(std::size_t index) const;

    private:
        ProjectInfo readProject(
            const std::filesystem::path& directory
        ) const;
        std::vector<ProjectInfo> projects;
    };
}