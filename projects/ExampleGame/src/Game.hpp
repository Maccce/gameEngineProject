#pragma once

#include "Project.hpp"

namespace ExampleGame
{
    class Game : public App::Project
    {
    public:
        void onInitialize() override;

        void onUpdate() override;

        void onShutdown() override;
    };
}
