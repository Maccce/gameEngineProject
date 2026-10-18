#include "Application.hpp"

#include "core/Time.hpp"

#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <utility>

namespace App
{
    namespace
    {
        Engine::Error makeError(
            Engine::ErrorCode code,
            std::string message
        )
        {
            return Engine::Error{
                code,
                std::move(message),
                nullptr
            };
        }

        void printError(const Engine::Error& error)
        {
            const Engine::Error* current = &error;

            while (current)
            {
                std::cerr << current->message << '\n';

                current = current->cause.get();
            }
        }
    }

    Application::Application()
    {
    }

    int Application::run()
    {
        auto initResult = initialize();

        if (!initResult)
        {
            std::cerr << "Application initialization failed:\n";
            printError(initResult.error());

            shutdown();
            return 1;
        }

        int frames = 0;

        while (running)
        {
            auto updateResult = update();

            if (!updateResult)
            {
                std::cerr << "Application update failed:\n";
                printError(updateResult.error());

                shutdown();
                return 1;
            }

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

    std::expected<void, Engine::Error> Application::initialize()
    {
        engine.initialize();
        projectRegistry.discover("projects");

        std::cout << "Available Projects\n\n";

        std::size_t index = 1;

        for (const auto& availableProject : projectRegistry.available())
        {
            std::cout
                << index++ << ". " << availableProject.name << '\n'
                << "   Description: " << availableProject.description << '\n'
                << "   Version: " << availableProject.version << '\n'
                << "   Path: " << availableProject.path << "\n\n";
        }

        if (projectRegistry.available().empty())
        {
            return std::unexpected(makeError(
                Engine::ErrorCode::NotFound,
                "No projects are available."
            ));
        }

        std::cout << "Select project: ";

        std::string input;

        if (!std::getline(std::cin >> std::ws, input))
        {
            return std::unexpected(makeError(
                Engine::ErrorCode::InvalidArgument,
                "Failed to read the project selection."
            ));
        }

        std::istringstream inputStream(input);
        std::size_t selection = 0;
        char extraCharacter = '\0';

        if (!(inputStream >> selection)
            || (inputStream >> extraCharacter))
        {
            return std::unexpected(makeError(
                Engine::ErrorCode::InvalidArgument,
                "Please enter a valid project number."
            ));
        }

        if (selection == 0
            || selection > projectRegistry.available().size())
        {
            return std::unexpected(makeError(
                Engine::ErrorCode::InvalidArgument,
                "Invalid project selection."
            ));
        }

        const auto& selectedProject =
            projectRegistry.project(selection - 1);

        std::cout << "\nSelected: " << selectedProject.name << '\n';

        Engine::Time::initialize();

        auto loadResult = projectLoader.load(selectedProject);

        if (!loadResult)
        {
            return std::unexpected(loadResult.error());
        }

        project = std::move(*loadResult);

        auto projectInitResult = project->initialize();

        if (!projectInitResult)
        {
            return std::unexpected(projectInitResult.error());
        }

        return {};
    }

    std::expected<void, Engine::Error> Application::update()
    {
        if (!project)
        {
            stop();

            return std::unexpected(makeError(
                Engine::ErrorCode::InvalidState,
                "Cannot update because no project is loaded."
            ));
        }

        Engine::Time::update();

        auto updateResult = project->update(Engine::Time::deltaTime());

        if (!updateResult)
        {
            return std::unexpected(updateResult.error());
        }

        std::cout
            << "Frame delta: "
            << Engine::Time::deltaTime()
            << '\n';

        return {};
    }

    void Application::shutdown()
    {
        if (project)
        {
            auto shutdownResult = project->shutdown();

            if (!shutdownResult)
            {
                std::cerr << "Project shutdown failed:\n";
                printError(shutdownResult.error());
            }

            project.reset();
        }

        engine.shutdown();
    }

    void Application::stop()
    {
        running = false;
    }
}
