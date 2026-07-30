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

    bool isRunning() const;


private:

    bool running = false;

};

}
