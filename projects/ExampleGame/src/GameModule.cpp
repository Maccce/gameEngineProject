#include "Game.hpp"

namespace GameModule
{
    App::Project* createProject()
    {
        return new ExampleGame::Game();
    }
}
