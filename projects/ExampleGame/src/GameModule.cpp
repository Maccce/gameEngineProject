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

    void initializeProject(AppProjectHandle* project)
    {
        if (!project)
            return;

        project->game.onInitialize();
    }

    void updateProject(AppProjectHandle* project, double deltaTime)
    {
        if (!project)
            return;

        (void)deltaTime;
        project->game.onUpdate();
    }

    void shutdownProject(AppProjectHandle* project)
    {
        if (!project)
            return;

        project->game.onShutdown();
    }

    void destroyProject(AppProjectHandle* project)
    {
        delete project;
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
