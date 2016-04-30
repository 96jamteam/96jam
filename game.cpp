#include "game.h"

#include "test_system.hpp"
#include "keyboardManager.h"
#include "structures_for_broadcast.h"
#include "menu_system.h"
#include "z_system.h"
#include "contact_name_cmp.h"

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
    engine.add(std::shared_ptr<System>(new WindowSystem(window)));
    engine.add(std::shared_ptr<System>(new SceneSystem(container)));
    engine.add(std::shared_ptr<System>(new TestSystem(window, this)));
    engine.add(std::shared_ptr<System>(new MenuSystem(&window, &container)));
    engine.add(std::shared_ptr<System>(new ZSystem(&window, &container, &views)));
    
    KeyboardManager keyboardmanager;
    keyboardmanager.load("controls.txt");
    engine.add(std::shared_ptr<System>(new SoundSystem()));
    EventChannel chan;
    chan.broadcast(PlaySound("electro.wav"));
    chan.broadcast(AddMusic("footsteps.ogg"));
    chan.broadcast(StartMusic());
    
    Stylesheets.addTranslation("what", "style.xml");
    Stylesheets.Get("what");
    
    
    views.VIEW_WIDTH = 800;
    views.VIEW_HEIGHT = 600;
    
    views.guiView.setSize(views.VIEW_WIDTH, views.VIEW_HEIGHT);
    views.guiView.setCenter(views.guiView.getSize().x / 2.0, views.guiView.getSize().y / 2.0);
    
    views.gameView = views.guiView = views.getLetterboxView(views.guiView, views.WINDOW_WIDTH, views.WINDOW_HEIGHT);
    
    int sceneID = 0;
    
    int entity = container.getUniqueID();
    
    container.createComponent<Scene>(entity);
    container.getComponent<Scene>(entity)->sceneID = sceneID;
    container.createComponent<ContactName>(entity);
    container.getComponent<ContactName>(entity)->name = "sprites";
    container.createComponent<SpriteC>(entity);
    
    sf::Sprite sp;
    sp.setTexture(*Textures.Get("gawron"));
    container.getComponent<SpriteC>(entity)->sprites.push_back(sp);
    container.getComponent<SpriteC>(entity)->z.push_back(0);
    sf::Sprite sp2;
    sp2.setTexture(*Textures.Get("gawron"));
    sp2.setPosition(100, 0);
    container.getComponent<SpriteC>(entity)->sprites.push_back(sp2);
    container.getComponent<SpriteC>(entity)->z.push_back(1);
    
    chan.broadcast(SpriteAdded());
    
    
    engine.run();
}
