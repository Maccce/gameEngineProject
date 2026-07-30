#include "core/Application.hpp"
#include "core/Log.hpp"


int main()
{
    Engine::Log::initialize();


    Engine::Application app;


    int result = app.run();


    Engine::Log::shutdown();


    return result;
}
