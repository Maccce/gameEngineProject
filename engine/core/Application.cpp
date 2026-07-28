#include "Application.hpp"


namespace Engine
{


Application::Application()
{

}


int Application::run()
{
    engine.initialize();


    // Main loop will go here later


    engine.shutdown();


    return 0;
}


}
