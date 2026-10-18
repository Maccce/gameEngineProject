#include "Application.hpp"

#include "core/Time.hpp"

#include <chrono>
#include <iostream>
#include <thread>

namespace App
{
    Application::Application()
    {
    }

    int Application::run()
    {
        initialize();

        int frames = 0;

        while (running)
        {
            update();
            frames++;

            if (frames > 10)
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
        projectRegistry.discover("projects");

        std::cout << "Available Projects\n\n";

        std::size_t index = 1;

        for (const auto& project : projectRegistry.available())
        {
            std::cout
                << index++ << ". " << project.name << '\n'
                << "   Description: " << project.description << '\n'
                << "   Version: " << project.version << '\n'
                << "   Path: " << project.path << "\n\n";
        }

        if (projectRegistry.available().empty())
        {
            std::cerr << "No projects available.\n";
            stop();
            return;
        }

        std::cout << "Select project: ";

        std::size_t selection = 0;
        std::cin >> selection;

        if (!std::cin
            || selection == 0
            || selection > projectRegistry.available().size())
        {
            std::cerr << "Invalid project selection.\n";
            stop();
            return;
        }

        const auto& selectedProject =
            projectRegistry.project(selection - 1);

        std::cout << "\nSelected: " << selectedProject.name << '\n';

        Engine::Time::initialize();

        project = projectLoader.load(selectedProject);

        if (!project)
        {
            stop();
            return;
        }

        project->initialize();
    }

    void Application::update()
    {
        if (!project)
        {
            stop();
            return;
        }

        Engine::Time::update();

        project->update(Engine::Time::deltaTime());

        std::cout
            << "Frame delta: "
            << Engine::Time::deltaTime()
            << '\n';
    }

    void Application::shutdown()
    {
        if (project)
        {
            project->shutdown();
            project.reset();
        }

        engine.shutdown();
    }

    void Application::stop()
    {
        running = false;
    }
}
