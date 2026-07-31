#include "Game.hpp"

#include <iostream>

namespace ExampleGame
{

void Game::onInitialize()
{
    std::cout << "ExampleGame initialized\n";
}


void Game::onUpdate()
{
    std::cout << "ExampleGame update\n";
}


void Game::onShutdown()
{
    std::cout << "ExampleGame shutdown\n";
}

}
