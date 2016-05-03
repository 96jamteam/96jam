#include "game.h"

#include "test_system.hpp"
#include "keyboardManager.h"
#include "structures_for_broadcast.h"
#include "menu_system.h"
#include "z_system.h"
#include "contact_name_cmp.h"

#include "player_system.h"
#include "debug_draw_system.h"
#include "bullet_system.h"
#include "bot_system.h"
#include "particle_system.h"
#include "remove_system.h"
#include "puzzle_system.h"
#include "particle_creator.h"

#include <fstream>

Game::Game() : contactlistener(ContactListener(&container))
{
    worldLoader = std::make_shared<WorldLoader>( this );
}

void Game::run()
{
    b2Vec2 Gravity(0.f, 0.f);

#ifdef __APPLE__
    world = new b2World(Gravity);
    world->SetAllowSleeping(false);
#else
    world = new b2World(Gravity, 0);
#endif

    SceneManager::init();

    world->SetContactListener(&contactlistener);

    createWindowAndStuff();
    std::shared_ptr<SoundSystem> sound(new SoundSystem());
    engine.add(std::shared_ptr<System>(new WindowSystem(window,views,playlists)));
    engine.add(std::shared_ptr<System>(new PuzzleSystem(container)));
    engine.add(std::shared_ptr<System>(new SceneSystem(container)));
    engine.add(std::shared_ptr<System>(new TestSystem(window, this)));
    engine.add(std::shared_ptr<System>(new MenuSystem(&window, &container)));
    engine.add(std::shared_ptr<System>(new PlayerSystem(&window, &container, this, &views)));
    engine.add(std::shared_ptr<System>(new WeaponSystem(&container,world)));
    engine.add(std::shared_ptr<System>(new BulletSystem(&container,world)));
    engine.add(std::shared_ptr<System>(new PhysicsSystem(world,container)));
    engine.add(std::shared_ptr<System>(new ZSystem(&window, &container, &views)));
    engine.add(sound);
    engine.add(std::shared_ptr<System>(new BotSystem(&container,world)));
    engine.add(std::shared_ptr<System>(new ParticleSystem(&container)));

    engine.add(std::shared_ptr<System>(new DebugDrawSystem(world, &window, &views)));
    engine.add(std::shared_ptr<System>(new RemoveSystem(&container)));

    EventChannel chan;

    loadAssets("assets//assets.xml");
    createMenus();
    createGameOverScreen();
    createWinScreen();
    createParticleFormula();
    createCreditsScreen();

    chan.broadcast(SpriteAdded());
    chan.broadcast(SceneUpdate());
    //chan.add<PlayerShooting>(*this);
    //mChannel.broadcast(StopMusic());
    playlists["menu"].set();
    mChannel.broadcast(StartMusic());

    engine.run();
    sound->destroy();
}

void Game::loadAssets(const std::string& path)
{
    XML xml;
    xml.load(path);
    for(auto& asset_xml : xml.iter("assets"))
    {
        if (asset_xml->getName() == "font")
        {
            fonts.addTranslation(asset_xml->get<std::string>(":name"), asset_xml->get<std::string>(":path"));
            fonts.Get(asset_xml->get<std::string>(":name"));
        }
        else if(asset_xml->getName() == "stylesheet")
        {
            Stylesheets.addTranslation(asset_xml->get<std::string>(":name"), asset_xml->get<std::string>(":path"));
            Stylesheets.Get(asset_xml->get<std::string>(":name"));
        }
        else if(asset_xml->getName() == "texture")
        {
            Textures.addTranslation(asset_xml->get<std::string>(":name"), asset_xml->get<std::string>(":path"));
            Textures.Get(asset_xml->get<std::string>(":name"));
        }
        else if(asset_xml->getName() == "animation")
        {
            Animations.addFromPath(asset_xml->get<std::string>(":path"), Textures);
        }
        else if (asset_xml->getName() == "music")
        {
            if(playlists.find(asset_xml->get<std::string>(":playlist"))==playlists.end())
                playlists[asset_xml->get<std::string>(":playlist")]=Playlist();
            playlists[asset_xml->get<std::string>(":playlist")].tracks.push_back(asset_xml->get<std::string>(":path"));
        }
    }


}


void Game::createParticleFormula(){
    ParticleCreator::addFormula(0,[this]()->int {

        int entityID = container.getUniqueID();
			container.createComponent<Transform>(entityID);
            container.createComponent<Scene>(entityID);
            container.createComponent<Particle>(entityID);
            container.getComponent<Scene>(entityID)->sceneID = SceneManager::getID("game");
            container.getComponent<Particle>(entityID)->lifespan = 0.08;
            container.createComponent<SpriteC>(entityID);
            container.getComponent<SpriteC>(entityID)->sprites.push_back(sf::Sprite(*Textures.Get("particle0")));
            container.getComponent<SpriteC>(entityID)->sprites[0].setOrigin(sf::Vector2f((*Textures.Get("particle0")).getSize().x/2.f,(*Textures.Get("particle0")).getSize().y/2.f));
            //container.getComponent<SpriteC>(entityID)->sprites[0].setScale(0.02f, 0.02f);
            container.getComponent<SpriteC>(entityID)->z.push_back(15);
            mChannel.broadcast(SpriteAdded());
        return entityID; });

    /*[this]()
    {
        mChannel.broadcast(PlaySound("assets/music/click.wav"));
       // mChannel.broadcast(MenuEvent("0", MenuEvent::hide));
        mChannel.broadcast(Engine::StopEvent());
    }*/
}


void Game::createMenus()
{

    int sceneID = SceneManager::addScene("menu", SceneManager::State::active);
    GuiStyle* gui;
    gui = Stylesheets.Get("default");
    gui->font = fonts.Get(gui->fontName);

    GuiStyle* gui2;
    gui2 = Stylesheets.Get("no_highlight");
    gui2->font = fonts.Get(gui2->fontName);

    int ID = container.getUniqueID();
    container.createComponent<Transform>(ID);

    container.createComponent<Menu>(ID);

    container.createComponent<Scene>(ID);

    container.getComponent<Scene>(ID)->sceneID = sceneID;

    container.getComponent<Transform>(ID)->x = 100;
    container.getComponent<Transform>(ID)->y = views.VIEW_HEIGHT / 2.f;
    int xsize = 500;
    int dim_y = 128;
    container.getComponent<Menu>(ID)->name = "main";
    container.getComponent<Menu>(ID)->z = 10000;

    MenuFactory::get().addScreen(*container.getComponent<Menu>(ID), "main");
    MenuFactory::get().addScreen(*container.getComponent<Menu>(ID), "options");

    MenuFactory::get().addGui(*
                              container.getComponent<Menu>(ID), "main", sf::Vector2f(0, dim_y * 1.5), sf::Vector2f(xsize, dim_y), 4, false, *gui,
    { std::make_pair("Play", "start"), std::make_pair("Options", "options_msg"), std::make_pair("Quit", "quit_msg") });

    MenuFactory::get().addGui(*
                              container.getComponent<Menu>(ID), "options", sf::Vector2f(0, dim_y * 2), sf::Vector2f(1370, dim_y), 4, false, *gui,
    { std::make_pair("Reset Game Saves", "reset_msg"),std::make_pair("Set Controls To: WSAD", "wsad_msg"),std::make_pair("Set Controls To: Arrows", "arrows_msg"),std::make_pair("Back", "back_msg") });


    MenuFactory::get().addGui(*
                              container.getComponent<Menu>(ID), "options", sf::Vector2f(0, 512.0 * 1), sf::Vector2f(xsize, dim_y/2.f), 4, false, *gui2,
    { std::make_pair("To change the volume edit the config file and restart the game", "ok") });


    MenuFactory::get().addConnection(*container.getComponent<Menu>(ID), "main", "options_msg", "options");
    MenuFactory::get().addConnection(*container.getComponent<Menu>(ID), "options", "back_msg", "main");

    MenuFactory::get().addAction(*container.getComponent<Menu>(ID), "main", "quit_msg",
                                 [this]()
    {
        mChannel.broadcast(PlaySound("assets/music/click.wav"));
       // mChannel.broadcast(MenuEvent("0", MenuEvent::hide));
        mChannel.broadcast(Engine::StopEvent());
    });

    MenuFactory::get().addAction(*container.getComponent<Menu>(ID), "options", "reset_msg",
                                 [this]()
    {
        mChannel.broadcast(PlaySound("assets/music/click.wav"));
        std::ofstream outfile;
        outfile.open("levels//save.xml");
        outfile << std::string(" <save level=\"") <<0<<std::string("\"></save> ") << std::endl;
        outfile.close();
    });

    MenuFactory::get().addAction(*container.getComponent<Menu>(ID), "options", "arrows_msg",
                                 [this]()
    {
        mChannel.broadcast(PlaySound("assets/music/click.wav"));
        std::ofstream outfile;
        outfile.open("controls.txt");
        outfile << std::string(" <key name=\"left\" keycode=\"71\"></key><key name=\"right\" keycode=\"72\"></key><key name=\"up\" keycode=\"73\"></key><key name=\"down\" keycode=\"74\"></key> ") << std::endl;
        outfile.close();
        mChannel.broadcast(ControlsUpdate());
    });

    MenuFactory::get().addAction(*container.getComponent<Menu>(ID), "options", "wsad_msg",
                                 [this]()
    {
        mChannel.broadcast(PlaySound("assets/music/click.wav"));
        std::ofstream outfile;
        outfile.open("controls.txt");
        outfile << std::string(" <key name=\"left\" keycode=\"0\"></key><key name=\"right\" keycode=\"3\"></key><key name=\"up\" keycode=\"22\"></key><key name=\"down\" keycode=\"18\"></key> ") << std::endl;
        outfile.close();
        mChannel.broadcast(ControlsUpdate());
    });

    MenuFactory::get().addAction(*container.getComponent<Menu>(ID), "main", "start",
                                 [this]()
    {
        mChannel.broadcast(PlaySound("assets/music/click.wav"));
        //mChannel.broadcast(PlaySound("assets/music/electro.wav"));

        //SceneManager::set(SceneManager::State::active, SceneManager::State::sleep);
        SceneManager::modState("menu", SceneManager::State::sleep);
        mChannel.broadcast(LoadWorld("normal", SceneManager::addScene("game", SceneManager::State::active)));
        mChannel.broadcast(StopMusic());
        playlists["game"].set();
        mChannel.broadcast(StartMusic());
    });

    MenuFactory::get().addAction(*container.getComponent<Menu>(ID), "main", "options_msg",
                                 [this]()
    {
        mChannel.broadcast(PlaySound("assets/music/click.wav"));
    });

    MenuFactory::get().addAction(*container.getComponent<Menu>(ID), "options", "back_msg",
                                 [this]()
    {
        mChannel.broadcast(PlaySound("assets/music/click.wav"));
    });

    MenuFactory::get().setActualScreen(*container.getComponent<Menu>(ID), "main");

}

void Game::createWinScreen()
{
    int sceneID = SceneManager::addScene("win_scene", SceneManager::State::sleep);
    GuiStyle* gui;
    gui = Stylesheets.Get("default");
    gui->font = fonts.Get(gui->fontName);

    GuiStyle* gui2;
    gui2 = Stylesheets.Get("no_highlight");
    gui2->font = fonts.Get(gui2->fontName);

    int ID = container.getUniqueID();
    container.createComponent<Transform>(ID);

    container.createComponent<Menu>(ID);

    container.createComponent<Scene>(ID);

    container.getComponent<Scene>(ID)->sceneID = sceneID;

    container.getComponent<Transform>(ID)->x = 100;
    container.getComponent<Transform>(ID)->y = views.VIEW_HEIGHT / 2.f;
    int xsize = 570;
    int dim_y = 128;
    container.getComponent<Menu>(ID)->name = "win_menu";
    container.getComponent<Menu>(ID)->z = 10000;

    MenuFactory::get().addScreen(*container.getComponent<Menu>(ID), "main");
    MenuFactory::get().addGui(*
                              container.getComponent<Menu>(ID), "main", sf::Vector2f(0, dim_y * 1), sf::Vector2f(xsize, dim_y), 4, false, *gui,
    {  std::make_pair("Next level", "next_msg"), std::make_pair("Quit", "quit_msg") });

    MenuFactory::get().addGui(*
                              container.getComponent<Menu>(ID), "main", sf::Vector2f(0, 500), sf::Vector2f(500, 256), 4, false, *gui2,
    { std::make_pair("U  REKT DEM"," die_msg") });
    /*MenuFactory::get().addGui(*
                        container.getComponent<Menu>(ID), "options", sf::Vector2f(0, 64.0 * 0.5), sf::Vector2f(xsize, 64), 4, false, *gui,
    { std::make_pair("Back", "back_msg") });*/
    //MenuFactory::get().addConnection(*container.getComponent<Menu>(ID), "main", "options_msg", "options");
    //MenuFactory::get().addConnection(*container.getComponent<Menu>(ID), "options", "back_msg", "main");
    //MenuFactory::get().addConnection(*container.getComponent<Menu>(ID), "main", "start", "options");

    MenuFactory::get().addAction(*container.getComponent<Menu>(ID), "main", "quit_msg",
                                 [this]()
    {
        //mChannel.broadcast(PlaySound("click.wav"));
        //mChannel.broadcast(MenuEvent("0", MenuEvent::hide));
        mChannel.broadcast(Engine::StopEvent());
    });

    MenuFactory::get().addAction(*container.getComponent<Menu>(ID), "main", "next_msg",
                                 [this]()
    {
        //mChannel.broadcast(PlaySound("electro.wav"));
        //SceneManager::modState("game",SceneManager::destroy);
        SceneManager::set(SceneManager::State::active, SceneManager::State::sleep);
        mChannel.broadcast(LoadWorld("next", SceneManager::addScene("game", SceneManager::State::active)));
    });

    MenuFactory::get().setActualScreen(*container.getComponent<Menu>(ID), "main");
}

void Game::createGameOverScreen()
{
    int sceneID = SceneManager::addScene("game_over", SceneManager::State::sleep);
    GuiStyle* gui;
    gui = Stylesheets.Get("default");
    gui->font = fonts.Get(gui->fontName);

    GuiStyle* gui2;
    gui2 = Stylesheets.Get("no_highlight");
    gui2->font = fonts.Get(gui2->fontName);

    int ID = container.getUniqueID();
    container.createComponent<Transform>(ID);

    container.createComponent<Menu>(ID);

    container.createComponent<Scene>(ID);

    container.getComponent<Scene>(ID)->sceneID = sceneID;

    container.getComponent<Transform>(ID)->x = 100;
    container.getComponent<Transform>(ID)->y = views.VIEW_HEIGHT / 2.f;
    int xsize = 450;
    int dim_y = 128;
    container.getComponent<Menu>(ID)->name = "main";
    container.getComponent<Menu>(ID)->z = 10000;

    MenuFactory::get().addScreen(*container.getComponent<Menu>(ID), "main");
    MenuFactory::get().addGui(*
                              container.getComponent<Menu>(ID), "main", sf::Vector2f(0, dim_y * 1), sf::Vector2f(xsize, dim_y), 4, false, *gui,
    { std::make_pair("Restart", "restart_msg"), std::make_pair("Quit", "quit_msg") });

    MenuFactory::get().addGui(*
                              container.getComponent<Menu>(ID), "main", sf::Vector2f(0, 500), sf::Vector2f(500, 256), 4, false, *gui2,
    { std::make_pair("U DIED BRO"," kupa") });
    /*MenuFactory::get().addGui(*
                        container.getComponent<Menu>(ID), "options", sf::Vector2f(0, 64.0 * 0.5), sf::Vector2f(xsize, 64), 4, false, *gui,
    { std::make_pair("Back", "back_msg") });*/
    //MenuFactory::get().addConnection(*container.getComponent<Menu>(ID), "main", "options_msg", "options");
    //MenuFactory::get().addConnection(*container.getComponent<Menu>(ID), "options", "back_msg", "main");
    //MenuFactory::get().addConnection(*container.getComponent<Menu>(ID), "main", "start", "options");

    MenuFactory::get().addAction(*container.getComponent<Menu>(ID), "main", "quit_msg",
                                 [this]()
    {
        //mChannel.broadcast(PlaySound("click.wav"));
        //mChannel.broadcast(MenuEvent("0", MenuEvent::hide));
        mChannel.broadcast(Engine::StopEvent());
    });

    MenuFactory::get().addAction(*container.getComponent<Menu>(ID), "main", "restart_msg",
                                 [this]()
    {
        //mChannel.broadcast(PlaySound("electro.wav"));
        SceneManager::modState("game",SceneManager::destroy);
        SceneManager::set(SceneManager::State::active, SceneManager::State::sleep);
        mChannel.broadcast(LoadWorld("normal", SceneManager::addScene("game", SceneManager::State::active)));
    });

    MenuFactory::get().setActualScreen(*container.getComponent<Menu>(ID), "main");
}

void Game::createCreditsScreen(){
    int sceneID = SceneManager::addScene("credits", SceneManager::State::sleep);
    int entityID = container.getUniqueID();

    container.createComponent<Transform>(entityID);
    container.createComponent<Scene>(entityID);
    container.createComponent<SpriteC>(entityID);

    container.getComponent<Scene>(entityID)->sceneID = sceneID;

    container.getComponent<Transform>(entityID)->x = 0;
    container.getComponent<Transform>(entityID)->y = 0;

    container.getComponent<SpriteC>(entityID)->sprites.push_back(sf::Sprite(*Textures.Get("credits")));
    container.getComponent<SpriteC>(entityID)->z.push_back(10000);

    mChannel.broadcast(SpriteAdded());
}

void Game::createWindowAndStuff()
{
    XML xml;
    xml.load("config.xml");

    views.WINDOW_WIDTH = xml.get<int>("config.window:width");
    views.WINDOW_HEIGHT = xml.get<int>("config.window:height");

    window.create(sf::VideoMode(views.WINDOW_WIDTH, views.WINDOW_HEIGHT), "7th96hJ");
    sf::ContextSettings settings = window.getSettings();
    std::cout << "OpenGl: "<<settings.majorVersion << "." << settings.minorVersion << std::endl;

    views.VIEW_WIDTH = xml.get<int>("config.view:width");
    views.VIEW_HEIGHT = xml.get<int>("config.view:height");

    views.guiView.setSize(views.VIEW_WIDTH, views.VIEW_HEIGHT);
    views.guiView.setCenter(views.guiView.getSize().x / 2.0, views.guiView.getSize().y / 2.0);

    views.gameView = views.guiView = views.getLetterboxView(views.guiView, views.WINDOW_WIDTH, views.WINDOW_HEIGHT);

    views.declareTextures(views.VIEW_WIDTH, views.VIEW_HEIGHT);
    engine.setVariables(&views, &window);
}
