#pragma once

#include "ProjectInfo.hpp"
#include "SharedLibrary.hpp"
#include "project_api.h"
#include "core/Error.hpp"

#include <expected>
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

        std::expected<void, Engine::Error> initialize();
        std::expected<void, Engine::Error> update(double deltaTime);
        std::expected<void, Engine::Error> shutdown();

    private:
        SharedLibrary library;
        const AppProjectAPI* api;
        AppProjectHandle* project;
        bool initialized = false;
    };

    class ProjectLoader
    {
    public:
        std::expected<std::unique_ptr<LoadedProject>, Engine::Error>
        load(const ProjectInfo& project);
    };
}
