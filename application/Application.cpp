#include "Application.hpp"

#include "core/Time.hpp"

#include <chrono>
#include <iostream>
#include <thread>


namespace App
{
    Application::Application(Project& project)
        :
        project(project)
    {
    }

    int Application::run()
    {
        initialize();
        int frames = 0;
        while(running)
        {
            update();
            frames++;
            if(frames > 10)
            {
                stop();
            }
            std::this_thread::sleep_for(
                std::chrono::milliseconds(100)
            );
        }
        shutdown();
        return 0;
    }

    void Application::initialize()
    {
        engine.initialize();
        Engine::Time::initialize();
        project.onInitialize();
    }

    void Application::update()
    {
        Engine::Time::update();
        project.onUpdate();
        std::cout
            << "Frame delta: "
            << Engine::Time::deltaTime()
            << "\n";
    }

    void Application::shutdown()
    {
        project.onShutdown();
        engine.shutdown();
    }

    void Application::stop()
    {
        running = false;
    }

}
