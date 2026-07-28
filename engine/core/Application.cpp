#include "Application.hpp"

#include "Time.hpp"

#include <iostream>
#include <thread>



namespace Engine
{


Application::Application()
{

}



void Application::update()
{
    std::cout
        << "Frame delta: "
        << Time::deltaTime()
        << "\n";
}



int Application::run()
{
    engine.initialize();


    Time::initialize();


    int frames = 0;


    while(running)
    {
        Time::update();


        update();


        frames++;


        // Temporary exit condition
        // until SDL provides events

        if(frames > 10)
        {
            running = false;
        }


        // Prevent max CPU usage

        std::this_thread::sleep_for(
            std::chrono::milliseconds(100)
        );
    }


    engine.shutdown();


    return 0;
}


}
