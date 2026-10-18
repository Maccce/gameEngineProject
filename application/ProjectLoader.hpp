#pragma once

#include "ProjectInfo.hpp"
#include "SharedLibrary.hpp"
#include "project_api.h"

#include <memory>

namespace App
{
    class LoadedProject
    {
    public:
        LoadedProject(
            SharedLibrary library,
            const AppProjectAPI* api,
            AppProjectHandle* project
        );

        ~LoadedProject();

        LoadedProject(const LoadedProject&) = delete;
        LoadedProject& operator=(const LoadedProject&) = delete;

        LoadedProject(LoadedProject&&) = delete;
        LoadedProject& operator=(LoadedProject&&) = delete;

        AppProjectHandle* get() const;

        void initialize();
        void update(double deltaTime);
        void shutdown();

    private:
        SharedLibrary library;
        const AppProjectAPI* api;
        AppProjectHandle* project;

        bool initialized = false;
    };

    class ProjectLoader
    {
    public:
        std::unique_ptr<LoadedProject> load(
            const ProjectInfo& project
        );
    };
}
