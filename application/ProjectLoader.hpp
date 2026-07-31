#pragma once

#include "Project.hpp"
#include "ProjectModule.hpp"

#include <memory>

namespace App
{
    class ProjectLoader
    {
    public:
        std::unique_ptr<Project> load();

    private:
        std::unique_ptr<ProjectModule> module;
    };
}
