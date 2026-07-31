#pragma once
#include "ProjectModule.hpp"

namespace ExampleGame
{
    class GameModule : public App::ProjectModule
    {
    public:
        App::Project* createProject() override;

        void destroyProject(App::Project* project) override;
    };
}
