#include "game.h"

#include "test_system.hpp"
#include "keyboardManager.h"
#include "structures_for_broadcast.h"

Game::Game()
{
    window.create(sf::VideoMode(800, 600,32), "7th96hJ",sf::Style::Default);
    sf::ContextSettings settings = window.getSettings();
    std::cout << "OpenGl: "<<settings.majorVersion << "." << settings.minorVersion << std::endl;
    Textures.addTranslation("gawron", "gawron.jpg");
    Textures.Get("gawron");
}

void Game::run()
{
    b2Vec2 Gravity(0.f, 9.81f);
	world = new b2World(Gravity,true);

    engine.add(std::shared_ptr<System>(new WindowSystem(window)));
    engine.add(std::shared_ptr<System>(new SceneSystem(container)));
    engine.add(std::shared_ptr<System>(new TestSystem(window, this)));
    engine.add(std::shared_ptr<System>(new PhysicsSystem(world,container)));
    KeyboardManager keyboardmanager;
    keyboardmanager.load("controls.txt");
    engine.add(std::shared_ptr<System>(new SoundSystem()));
    EventChannel chan;
    chan.broadcast(PlaySound("electro.wav"));
    chan.broadcast(AddMusic("footsteps.ogg"));
    chan.broadcast(StartMusic());
    engine.run();
}
