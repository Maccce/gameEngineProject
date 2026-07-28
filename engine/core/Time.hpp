#pragma once

#include <chrono>


namespace Engine
{

class Time
{
public:

    static void initialize();

    static void update();


    static float deltaTime();

    static float totalTime();


private:

    static std::chrono::high_resolution_clock::time_point startTime;

    static std::chrono::high_resolution_clock::time_point lastFrame;


    static float deltaSeconds;

    static float totalSeconds;
};

}
