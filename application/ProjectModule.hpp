#pragma once
#include "Project.hpp"

namespace App
{
class ProjectModule
    {
    public:
        virtual ~ProjectModule() = default;
        virtual Project* createProject() = 0;
        virtual void destroyProject(Project* project) = 0;
    };
}