#include "Engine.hpp"

#include "Log.hpp"


namespace Engine
{

EngineCore::EngineCore()
{
}


void EngineCore::initialize()
{
    Log::info("Engine initialized");

    running = true;
}


void EngineCore::shutdown()
{
    Log::info("Engine shutdown");

    running = false;
}

bool EngineCore::isRunning() const
{
    return running;
}

}
