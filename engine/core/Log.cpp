#include "Log.hpp"

#include <spdlog/spdlog.h>


namespace Engine
{

bool Log::initialized = false;



void Log::initialize()
{
    spdlog::set_pattern(
        "[%l] %v"
    );

    initialized = true;

    info("Logging initialized");
}



void Log::shutdown()
{
    info("Logging shutdown");

    initialized = false;
}



void Log::info(const char* message)
{
    spdlog::info(message);
}



void Log::warn(const char* message)
{
    spdlog::warn(message);
}



void Log::error(const char* message)
{
    spdlog::error(message);
}


}
