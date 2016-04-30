#include "game.h"

#include "test_system.hpp"
#include "keyboardManager.h"
#include "structures_for_broadcast.h"
#include "menu_system.h"
#include "z_system.h"
#include "contact_name_cmp.h"
#include "player_system.h"

Game::Game()
{
    worldLoader = std::make_shared<WorldLoader>( this );
    window.create(sf::VideoMode(800, 600,32), "7th96hJ",sf::Style::Default);
    sf::ContextSettings settings = window.getSettings();
    std::cout << "OpenGl: "<<settings.majorVersion << "." << settings.minorVersion << std::endl;
    Textures.addTranslation("gawron", "gawron.jpg");
    Textures.Get("gawron");
}

void Game::run()
{
    b2Vec2 Gravity(0.f, 9.81f);
	world = new b2World(Gravity);

    engine.add(std::shared_ptr<System>(new WindowSystem(window,views)));
    engine.add(std::shared_ptr<System>(new SceneSystem(container)));
    engine.add(std::shared_ptr<System>(new TestSystem(window, this)));
    engine.add(std::shared_ptr<System>(new MenuSystem(&window, &container)));
    engine.add(std::shared_ptr<System>(new ZSystem(&window, &container, &views)));
    engine.add(std::shared_ptr<System>(new PhysicsSystem(world,container)));
    engine.add(std::shared_ptr<System>(new PlayerSystem(&window, &container, this, &views)));
    engine.add(std::shared_ptr<System>(new SoundSystem()));

    KeyboardManager keyboardmanager;
    keyboardmanager.load("controls.txt");

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
    views.declareTextures(views.VIEW_WIDTH, views.VIEW_HEIGHT);
    engine.setVariables(&views, &window);

    int sceneID = SceneManager::addScene("game", SceneManager::State::active);

    int entity = container.getUniqueID();

    container.createComponent<Scene>(entity);
    container.getComponent<Scene>(entity)->sceneID = sceneID;
    container.createComponent<ContactName>(entity);
    container.getComponent<ContactName>(entity)->name = "sprites";
    container.createComponent<SpriteC>(entity);
    container.createComponent<Transform>(entity);
    container.getComponent<Transform>(entity)->x = 0;
    container.getComponent<Transform>(entity)->y = 0;
    container.getComponent<Transform>(entity)->angle = 0;

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
    chan.broadcast(SceneUpdate());


    engine.run();
}
