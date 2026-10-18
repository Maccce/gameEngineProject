#pragma once

#include "core/Engine.hpp"
#include "ProjectLoader.hpp"
#include "ProjectRegistry.hpp"

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
        ProjectRegistry projectRegistry;
        ProjectLoader projectLoader;

        std::unique_ptr<LoadedProject> project;
    };
}
