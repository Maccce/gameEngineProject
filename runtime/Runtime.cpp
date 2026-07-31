#include "ProjectRegistry.hpp"
#include "core/Log.hpp"

#include <iostream>

int main()
{
    Engine::Log::initialize();

    App::ProjectRegistry registry;

    registry.discover("projects");

    std::cout << "Available Projects\n\n";

    std::size_t index = 1;

    for (const auto& project : registry.available())
    {
        std::cout
            << index++ << ". " << project.name << '\n'
            << "   Description: " << project.description << '\n'
            << "   Version: " << project.version << '\n'
            << "   Path: " << project.path << "\n\n";
    }

    std::cout << "Select project: ";

    std::size_t selection;
    std::cin >> selection;

    const auto& project = registry.project(selection - 1);

    std::cout << "\nSelected: " << project.name << '\n';

    Engine::Log::shutdown();

    return 0;
}
