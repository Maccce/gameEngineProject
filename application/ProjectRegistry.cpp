#include "ProjectRegistry.hpp"

#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

namespace App
{
    void ProjectRegistry::discover(
        const std::filesystem::path& projectsDirectory
    )
    {
        projects.clear();
        for (const auto& entry : std::filesystem::directory_iterator(projectsDirectory))
        {
            if (!entry.is_directory())
            {
                continue;
            }
            const auto metadata = entry.path() / "project.json";
            if (!std::filesystem::exists(metadata))
            {
                continue;
            }
            projects.push_back(
                readProject(entry.path())
            );
        }
    }

    ProjectInfo ProjectRegistry::readProject(
        const std::filesystem::path& directory
    ) const
    {
        std::ifstream file(directory / "project.json");
        nlohmann::json json;
        file >> json;
        ProjectInfo info;
        info.name = json["name"];
        info.description = json["description"];
        info.version = json["version"];
        info.path = directory;
        return info;
    }

    const std::vector<ProjectInfo>& ProjectRegistry::available() const
    {
        return projects;
    }

    const ProjectInfo& ProjectRegistry::project(
        std::size_t index
    ) const
    {
        if (index >= projects.size())
        {
            throw std::out_of_range(
                "Invalid project index."
            );
        }
        return projects[index];
    }
}