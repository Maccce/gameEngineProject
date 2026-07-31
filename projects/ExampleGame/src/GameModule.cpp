#include "GameModule.hpp"
#include "Game.hpp"

namespace ExampleGame
{
    App::Project* GameModule::createProject()
    {
        return new Game();
    }

    void GameModule::destroyProject(App::Project* project)
    {
        delete project;
    }
}
