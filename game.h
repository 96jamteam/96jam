#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include "engine.h"
#include "component_container.h"
#include "window_system.h"
#include "pugi_wrapper.h"

class Game
{
public:
    Game();
    void run()
    {
        engine.add(std::shared_ptr<System>(new WindowSystem(window)));
        engine.run();
    }

    sf::RenderWindow window;
    Engine engine;
    ComponentContainer container;
};

#endif // GAME_H
