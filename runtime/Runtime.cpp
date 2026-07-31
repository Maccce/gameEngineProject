#include "Application.hpp"
#include "GameModule.hpp"
#include "core/Log.hpp"


int main()
{
    Engine::Log::initialize();

    App::Project* project = GameModule::createProject();

    App::Application app(*project);

    int result = app.run();

    delete project;

    Engine::Log::shutdown();

    return result;
}
