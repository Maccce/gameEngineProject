#include "ProjectLoader.hpp"

#include <filesystem>
#include <iostream>
#include <memory>
#include <utility>

namespace App
{
    LoadedProject::LoadedProject(
        SharedLibrary library,
        ProjectModule* module,
        Project* project
    )
        : library(std::move(library)),
          module(module),
          project(project)
    {
    }

    LoadedProject::~LoadedProject()
    {
        if (project)
        {
            module->destroyProject(project);
            project = nullptr;
        }

        delete module;
        module = nullptr;

        library.unload();
    }

    Project* LoadedProject::get() const
    {
        return project;
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
        {
            return nullptr;
        }

        using CreateProjectModule = ProjectModule* (*)();

        auto createModule =
            reinterpret_cast<CreateProjectModule>(
                library.getSymbol("createProjectModule")
            );

        if (!createModule)
        {
            return nullptr;
        }

        std::unique_ptr<ProjectModule> module(createModule());

        if (!module)
        {
            std::cerr << "Failed to create project module.\n";
            return nullptr;
        }

        Project* createdProject = module->createProject();

        if (!createdProject)
        {
            std::cerr << "Failed to create project instance.\n";
            return nullptr;
        }

        std::cout << "Successfully created project instance.\n";

        auto loadedProject = std::make_unique<LoadedProject>(
            std::move(library),
            module.get(),
            createdProject
        );

        module.release();

        return loadedProject;
    }
}
