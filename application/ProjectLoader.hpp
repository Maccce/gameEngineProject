#pragma once

#include "Project.hpp"
#include "ProjectInfo.hpp"
#include "ProjectModule.hpp"
#include "SharedLibrary.hpp"

#include <memory>

namespace App
{
    class LoadedProject
    {
    public:
        LoadedProject(
            SharedLibrary library,
            ProjectModule* module,
            Project* project
        );

        ~LoadedProject();

        LoadedProject(const LoadedProject&) = delete;
        LoadedProject& operator=(const LoadedProject&) = delete;

        LoadedProject(LoadedProject&&) = delete;
        LoadedProject& operator=(LoadedProject&&) = delete;

        Project* get() const;

    private:
        SharedLibrary library;
        ProjectModule* module;
        Project* project;
    };

    class ProjectLoader
    {
    public:
        std::unique_ptr<LoadedProject> load(
            const ProjectInfo& project
        );
    };
}
