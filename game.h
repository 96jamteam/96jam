#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include "engine.h"

class Game
{
public:
    Game();
    void run()
    {
        engine.run();
    }
    sf::RenderWindow window;
    Engine engine;
private:
};

#endif // GAME_H
