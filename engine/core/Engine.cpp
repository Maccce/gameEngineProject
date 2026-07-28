#include "Engine.hpp"

#include <iostream>


namespace Engine
{

EngineCore::EngineCore()
{
}


void EngineCore::initialize()
{
    std::cout << "Engine initialized\n";

    running = true;
}


void EngineCore::shutdown()
{
    std::cout << "Engine shutdown\n";

    running = false;
}

}
