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

    EngineCore engine;

};

}
