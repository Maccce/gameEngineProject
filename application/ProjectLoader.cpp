#include "ProjectLoader.hpp"

#include <filesystem>
#include <iostream>
#include <memory>
#include <utility>

namespace App
{
    namespace
    {
        struct ProjectHandleGuard
        {
            const AppProjectAPI* api;
            AppProjectHandle* project;

            ~ProjectHandleGuard()
            {
                if (project && api && api->destroy_project)
                    api->destroy_project(project);
            }

            void release()
            {
                project = nullptr;
            }
        };
    }

    LoadedProject::LoadedProject(
        SharedLibrary library,
        const AppProjectAPI* api,
        AppProjectHandle* project
    )
        : library(std::move(library)),
          api(api),
          project(project)
    {
    }

    LoadedProject::~LoadedProject()
    {
        shutdown();

        if (project && api && api->destroy_project)
        {
            api->destroy_project(project);
            project = nullptr;
        }

        api = nullptr;
        library.unload();
    }

    AppProjectHandle* LoadedProject::get() const
    {
        return project;
    }

    void LoadedProject::initialize()
    {
        if (!project || !api || initialized)
            return;

        api->initialize_project(project);
        initialized = true;
    }

    void LoadedProject::update(double deltaTime)
    {
        if (!project || !api || !initialized)
            return;

        api->update_project(project, deltaTime);
    }

    void LoadedProject::shutdown()
    {
        if (!project || !api || !initialized)
            return;

        api->shutdown_project(project);
        initialized = false;
    }

    std::unique_ptr<LoadedProject> ProjectLoader::load(
        const ProjectInfo& project
    )
    {
        std::cout << "Loading project: " << project.name << '\n';

        SharedLibrary library;

        const std::string libraryName =
            SharedLibrary::getPlatformLibraryName(
                project.path.filename().string()
            );

        const std::filesystem::path libraryPath =
            std::filesystem::current_path()
            / "build"
            / "lib"
            / libraryName;

        if (!library.load(libraryPath.string()))
            return nullptr;

        auto getProjectAPI =
            reinterpret_cast<AppGetProjectAPI>(
                library.getSymbol("getProjectAPI")
            );

        if (!getProjectAPI)
        {
            std::cerr << "Failed to find getProjectAPI symbol.\n";
            return nullptr;
        }

        const AppProjectAPI* api =
            getProjectAPI(APP_PROJECT_API_VERSION);

        if (!api)
        {
            std::cerr << "Project does not support this API version.\n";
            return nullptr;
        }

        if (api->api_version != APP_PROJECT_API_VERSION
            || !api->create_project
            || !api->initialize_project
            || !api->update_project
            || !api->shutdown_project
            || !api->destroy_project)
        {
            std::cerr << "Project API is invalid or incomplete.\n";
            return nullptr;
        }

        AppProjectHandle* createdProject = api->create_project();

        if (!createdProject)
        {
            std::cerr << "Failed to create project instance.\n";
            return nullptr;
        }

        ProjectHandleGuard guard{api, createdProject};

        std::cout << "Successfully created project instance.\n";

        auto loadedProject = std::make_unique<LoadedProject>(
            std::move(library),
            api,
            createdProject
        );

        guard.release();
        return loadedProject;
    }
}
