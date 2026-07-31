#pragma once

#include "core/Engine.hpp"
#include "Project.hpp"

namespace App
{
    class Application
    {
    public:
        explicit Application(Project& project);

        int run();

        void stop();

    private:
        void initialize();
        void shutdown();

        void update();

        bool running = true;

        Engine::EngineCore engine;

        Project& project;
    };
}
