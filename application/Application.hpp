#pragma once

#include "core/Engine.hpp"
#include "core/Error.hpp"
#include "ProjectLoader.hpp"
#include "ProjectRegistry.hpp"

#include <expected>
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
        std::expected<void, Engine::Error> initialize();
        void shutdown();
        std::expected<void, Engine::Error> update();

        bool running = true;

        Engine::EngineCore engine;
        ProjectRegistry projectRegistry;
        ProjectLoader projectLoader;

        std::unique_ptr<LoadedProject> project;
    };
}
