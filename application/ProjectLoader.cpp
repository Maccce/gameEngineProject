
#include "ProjectLoader.hpp"

#include <filesystem>
#include <memory>
#include <string>
#include <utility>

namespace App
{
    namespace
    {
        Engine::Error makeError(
            Engine::ErrorCode code,
            std::string message,
            std::shared_ptr<const Engine::Error> cause = nullptr
        )
        {
            return Engine::Error{
                code,
                std::move(message),
                std::move(cause)
            };
        }

        Engine::Error addContext(
            std::string message,
            const Engine::Error& cause
        )
        {
            return makeError(
                cause.code,
                std::move(message),
                std::make_shared<Engine::Error>(cause)
            );
        }

        Engine::Error makeCallbackError(const char* callback)
        {
            return makeError(
                Engine::ErrorCode::ExternalFailure,
                std::string("Project callback failed: ") + callback + "."
            );
        }

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
        // Destructors cannot report errors. Attempt shutdown, then
        // destroy the project while its library is still loaded.
        (void)shutdown();

        if (project && api && api->destroy_project)
        {
            api->destroy_project(project);
            project = nullptr;
        }

        api = nullptr;
        (void)library.unload();
    }

    AppProjectHandle* LoadedProject::get() const
    {
        return project;
    }

    std::expected<void, Engine::Error> LoadedProject::initialize()
    {
        if (!project || !api)
        {
            return std::unexpected(makeError(
                Engine::ErrorCode::InvalidState,
                "Cannot initialize an invalid project."
            ));
        }

        if (initialized)
            return {};

        const AppProjectStatus status =
            api->initialize_project(project);

        if (status != APP_PROJECT_STATUS_SUCCESS)
        {
            return std::unexpected(
                makeCallbackError("initialize_project")
            );
        }

        initialized = true;
        return {};
    }

    std::expected<void, Engine::Error> LoadedProject::update(
        double deltaTime
    )
    {
        if (!project || !api || !initialized)
        {
            return std::unexpected(makeError(
                Engine::ErrorCode::InvalidState,
                "Cannot update a project that is not initialized."
            ));
        }

        const AppProjectStatus status =
            api->update_project(project, deltaTime);

        if (status != APP_PROJECT_STATUS_SUCCESS)
        {
            return std::unexpected(
                makeCallbackError("update_project")
            );
        }

        return {};
    }

    std::expected<void, Engine::Error> LoadedProject::shutdown()
    {
        if (!project || !api)
        {
            return std::unexpected(makeError(
                Engine::ErrorCode::InvalidState,
                "Cannot shut down an invalid project."
            ));
        }

        if (!initialized)
            return {};

        // Do not retry shutdown automatically if the callback reports
        // failure; it may already have partially shut down the project.
        initialized = false;

        const AppProjectStatus status =
            api->shutdown_project(project);

        if (status != APP_PROJECT_STATUS_SUCCESS)
        {
            return std::unexpected(
                makeCallbackError("shutdown_project")
            );
        }

        return {};
    }

    std::expected<std::unique_ptr<LoadedProject>, Engine::Error>
    ProjectLoader::load(const ProjectInfo& project)
    {
        namespace fs = std::filesystem;

        SharedLibrary library;

        const std::string libraryName =
            SharedLibrary::getPlatformLibraryName(
                project.path.filename().string()
            );

        const fs::path libraryPath =
            fs::current_path()
            / "build"
            / "lib"
            / libraryName;

        auto loadResult = library.load(libraryPath.string());

        if (!loadResult)
        {
            return std::unexpected(addContext(
                "Failed to load project '" + project.name + "'.",
                loadResult.error()
            ));
        }

        auto symbolResult = library.getSymbol("getProjectAPI");

        if (!symbolResult)
        {
            return std::unexpected(addContext(
                "Failed to resolve the project API for '"
                    + project.name + "'.",
                symbolResult.error()
            ));
        }

        auto getProjectAPI =
            reinterpret_cast<AppGetProjectAPI>(*symbolResult);

        const AppProjectAPI* api =
            getProjectAPI(APP_PROJECT_API_VERSION);

        if (!api)
        {
            return std::unexpected(makeError(
                Engine::ErrorCode::UnsupportedVersion,
                "Project '" + project.name
                    + "' does not support API version "
                    + std::to_string(APP_PROJECT_API_VERSION) + "."
            ));
        }

        if (api->api_version != APP_PROJECT_API_VERSION
            || !api->create_project
            || !api->initialize_project
            || !api->update_project
            || !api->shutdown_project
            || !api->destroy_project)
        {
            return std::unexpected(makeError(
                Engine::ErrorCode::InvalidData,
                "Project '" + project.name
                    + "' provides an invalid or incomplete API."
            ));
        }

        AppProjectHandle* createdProject = api->create_project();

        if (!createdProject)
        {
            return std::unexpected(makeError(
                Engine::ErrorCode::ResourceCreationFailed,
                "Failed to create an instance of project '"
                    + project.name + "'."
            ));
        }

        ProjectHandleGuard guard{api, createdProject};

        auto loadedProject = std::make_unique<LoadedProject>(
            std::move(library),
            api,
            createdProject
        );

        guard.release();

        return loadedProject;
    }
}
