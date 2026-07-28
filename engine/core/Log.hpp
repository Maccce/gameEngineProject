#pragma once

#include <memory>


namespace Engine
{

class Log
{

public:

    static void initialize();

    static void shutdown();


    static void info(const char* message);

    static void warn(const char* message);

    static void error(const char* message);


private:

    static bool initialized;

};

}
