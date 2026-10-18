
#include "Game.hpp"
#include "project_api.h"

// The handle's implementation stays private to this shared library.
struct AppProjectHandle
{
    ExampleGame::Game game;
};

namespace ExampleGame
{
    AppProjectHandle* createProject()
    {
        try
        {
            return new AppProjectHandle{};
        }
        catch (...)
        {
            return nullptr;
        }
    }

    AppProjectStatus initializeProject(AppProjectHandle* project)
    {
        if (!project)
            return APP_PROJECT_STATUS_FAILURE;

        try
        {
            project->game.onInitialize();
            return APP_PROJECT_STATUS_SUCCESS;
        }
        catch (...)
        {
            return APP_PROJECT_STATUS_FAILURE;
        }
    }

    AppProjectStatus updateProject(
        AppProjectHandle* project,
        double deltaTime
    )
    {
        if (!project)
            return APP_PROJECT_STATUS_FAILURE;

        try
        {
            (void)deltaTime;
            project->game.onUpdate();
            return APP_PROJECT_STATUS_SUCCESS;
        }
        catch (...)
        {
            return APP_PROJECT_STATUS_FAILURE;
        }
    }

    AppProjectStatus shutdownProject(AppProjectHandle* project)
    {
        if (!project)
            return APP_PROJECT_STATUS_FAILURE;

        try
        {
            project->game.onShutdown();
            return APP_PROJECT_STATUS_SUCCESS;
        }
        catch (...)
        {
            return APP_PROJECT_STATUS_FAILURE;
        }
    }

    void destroyProject(AppProjectHandle* project)
    {
        // Destruction must not allow exceptions to escape the ABI.
        try
        {
            delete project;
        }
        catch (...)
        {
            // Destructors should not throw. There is no error return
            // available from this callback.
        }
    }

    const AppProjectAPI api{
        APP_PROJECT_API_VERSION,
        &createProject,
        &initializeProject,
        &updateProject,
        &shutdownProject,
        &destroyProject
    };
}

extern "C" const AppProjectAPI* getProjectAPI(uint8_t hostApiVersion)
{
    if (hostApiVersion != APP_PROJECT_API_VERSION)
        return nullptr;

    return &ExampleGame::api;
}
