#include "Time.hpp"


namespace Engine
{


std::chrono::high_resolution_clock::time_point Time::startTime;

std::chrono::high_resolution_clock::time_point Time::lastFrame;


float Time::deltaSeconds = 0.0f;

float Time::totalSeconds = 0.0f;



void Time::initialize()
{
    startTime =
        std::chrono::high_resolution_clock::now();

    lastFrame = startTime;
}



void Time::update()
{
    auto current =
        std::chrono::high_resolution_clock::now();


    deltaSeconds =
        std::chrono::duration<float>(
            current - lastFrame
        ).count();


    totalSeconds =
        std::chrono::duration<float>(
            current - startTime
        ).count();


    lastFrame = current;
}



float Time::deltaTime()
{
    return deltaSeconds;
}



float Time::totalTime()
{
    return totalSeconds;
}


}
