#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include "engine.h"
#include "component_container.h"
#include "window_system.h"
#include "sound_system.h"
#include "scene_system.h"
#include "physics_system.h"
#include "pugi_wrapper.h"
#include "ResourceManagerWrapper.hpp"

class TestSystem;

class Game
{
public:
    Game();

    void run();

    sf::RenderWindow window;
    Engine engine;
    ComponentContainer container;
    b2World* world;

    Manager<sf::Texture, 100> Textures;
};

#endif // GAME_H
