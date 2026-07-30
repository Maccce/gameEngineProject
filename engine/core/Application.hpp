#pragma once

#include "Engine.hpp"


namespace Engine
{

class Application
{

public:

    Application();

    int run();

    void stop();


private:

    bool running = true;


    void update();


    EngineCore engine;

};

}
