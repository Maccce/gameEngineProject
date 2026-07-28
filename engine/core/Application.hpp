#pragma once

#include "Engine.hpp"


namespace Engine
{

class Application
{

public:

    Application();

    int run();


private:

    bool running = true;


    void update();


    EngineCore engine;

};

}
