#pragma once

#include "core/Engine.hpp"
#include "Project.hpp"
#include "ProjectLoader.hpp"

#include <memory>

namespace App
{
    class Application
    {
    public:
        Application();

        int run();

        void stop();

    private:
        void initialize();
        void shutdown();

        void update();

        bool running = true;

        Engine::EngineCore engine;

        ProjectLoader projectLoader;

        std::unique_ptr<Project> project;
    };
}
