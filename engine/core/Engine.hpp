#pragma once

#include <string>


namespace Engine
{

class EngineCore
{
public:

    EngineCore();

    void initialize();

    void shutdown();


private:

    bool running = false;

};

}
