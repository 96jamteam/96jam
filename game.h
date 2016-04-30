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
#include "gui.h"
#include "views.h"
#include "AnimationManager.h"
#include "world_loader.h"

class TestSystem;
class PlayerSystem;

class Game
{
public:
    Game();

    void run();

    sf::RenderWindow window;
    Engine engine;
    ComponentContainer container;
    b2World* world;

    Manager<sf::Font, stuff::MAX_FONTS> fonts;
	Manager<GuiStyle, stuff::MAX_STYLESHEETS> Stylesheets;
	Manager<sf::Texture, stuff::MAX_TEXTURES> Textures;
	AnimationManager Animations;

    Views views;
    std::shared_ptr<WorldLoader> worldLoader;
};

#endif // GAME_H
