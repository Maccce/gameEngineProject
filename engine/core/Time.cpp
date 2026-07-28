#include "Time.hpp"


namespace Engine
{


std::chrono::high_resolution_clock::time_point Time::startTime;

std::chrono::high_resolution_clock::time_point Time::lastFrame;


float Time::delta = 0.0f;

float Time::total = 0.0f;



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


    delta =
        std::chrono::duration<float>(
            current - lastFrame
        ).count();


    total =
        std::chrono::duration<float>(
            current - startTime
        ).count();


    lastFrame = current;
}



float Time::deltaTime()
{
    return delta;
}



float Time::totalTime()
{
    return total;
}


}
