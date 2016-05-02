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

    engine.add(std::shared_ptr<System>(new WindowSystem(window,views)));
    engine.add(std::shared_ptr<System>(new PuzzleSystem(container)));
    engine.add(std::shared_ptr<System>(new SceneSystem(container)));
    engine.add(std::shared_ptr<System>(new TestSystem(window, this)));
    engine.add(std::shared_ptr<System>(new MenuSystem(&window, &container)));
    engine.add(std::shared_ptr<System>(new PlayerSystem(&window, &container, this, &views)));
    engine.add(std::shared_ptr<System>(new WeaponSystem(&container,world)));
    engine.add(std::shared_ptr<System>(new BulletSystem(&container,world)));
    engine.add(std::shared_ptr<System>(new PhysicsSystem(world,container)));
    engine.add(std::shared_ptr<System>(new ZSystem(&window, &container, &views)));
    engine.add(std::shared_ptr<System>(new SoundSystem()));
    engine.add(std::shared_ptr<System>(new BotSystem(&container,world)));
    engine.add(std::shared_ptr<System>(new ParticleSystem(&container)));

    engine.add(std::shared_ptr<System>(new DebugDrawSystem(world, &window, &views)));
    engine.add(std::shared_ptr<System>(new RemoveSystem(&container)));

    EventChannel chan;

    loadAssets("assets.xml");
    createMenus();
    createGameOverScreen();
    createWinScreen();

    chan.broadcast(SpriteAdded());
    chan.broadcast(SceneUpdate());

    engine.run();
}

 void Game::loadAssets(const std::string& path){
        XML xml;
        xml.load(path);
        for(auto& asset_xml : xml.iter("assets")){
            if (asset_xml->getName() == "font"){
                fonts.addTranslation(asset_xml->get<std::string>(":name"), asset_xml->get<std::string>(":path"));
				fonts.Get(asset_xml->get<std::string>(":name"));
            }else if(asset_xml->getName() == "stylesheet"){
				Stylesheets.addTranslation(asset_xml->get<std::string>(":name"), asset_xml->get<std::string>(":path"));
				Stylesheets.Get(asset_xml->get<std::string>(":name"));
            }else if(asset_xml->getName() == "texture"){
				Textures.addTranslation(asset_xml->get<std::string>(":name"), asset_xml->get<std::string>(":path"));
				Textures.Get(asset_xml->get<std::string>(":name"));
            }else if(asset_xml->getName() == "animation"){
                Animations.addFromPath(asset_xml->get<std::string>(":path"), Textures);
            }else if (asset_xml->getName() == "music") {
				mChannel.broadcast(AddMusic(asset_xml->get<std::string>(":path")));
			}
        }


    }

void Game::createMenus() {

		int sceneID = SceneManager::addScene("menu", SceneManager::State::active);
		GuiStyle* gui;
		gui = Stylesheets.Get("text");
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
		int xsize = 300;
		container.getComponent<Menu>(ID)->name = "main";
		container.getComponent<Menu>(ID)->z = 10000;

		MenuFactory::get().addScreen(*container.getComponent<Menu>(ID), "main");
		MenuFactory::get().addScreen(*container.getComponent<Menu>(ID), "options");
		MenuFactory::get().addGui(*
                            container.getComponent<Menu>(ID), "main", sf::Vector2f(0, 64.0 * 1.5), sf::Vector2f(xsize, 64), 4, false, *gui,
		{ std::make_pair("Play", "start"), std::make_pair("Options", "options_msg"), std::make_pair("Quit", "quit_msg") });
		MenuFactory::get().addGui(*
                            container.getComponent<Menu>(ID), "options", sf::Vector2f(0, 64.0 * 1), sf::Vector2f(xsize, 64), 4, false, *gui,
		{ std::make_pair("Reset Game Saves", "reset_msg"),std::make_pair("Back", "back_msg") });

		MenuFactory::get().addGui(*
                            container.getComponent<Menu>(ID), "options", sf::Vector2f(0, 512.0 * 1), sf::Vector2f(xsize, 64), 4, false, *gui2,
		{ std::make_pair("To change the volume edit the config file and restart the game", "ok") });


		MenuFactory::get().addConnection(*container.getComponent<Menu>(ID), "main", "options_msg", "options");
		MenuFactory::get().addConnection(*container.getComponent<Menu>(ID), "options", "back_msg", "main");
		MenuFactory::get().addConnection(*container.getComponent<Menu>(ID), "main", "start", "options");

		MenuFactory::get().addAction(*container.getComponent<Menu>(ID), "main", "quit_msg",
			[this]() {
			mChannel.broadcast(PlaySound("click.wav"));
			mChannel.broadcast(MenuEvent("0", MenuEvent::hide));
			mChannel.broadcast(Engine::StopEvent()); });

        MenuFactory::get().addAction(*container.getComponent<Menu>(ID), "options", "reset_msg",
                                    [this]() {
                                    std::ofstream outfile;
                                    outfile.open("levels//save.xml");
                                    outfile << std::string(" <save level=\"") <<0<<std::string("\"></save> ") << std::endl;
                                    outfile.close();
                                    });

		MenuFactory::get().addAction(*container.getComponent<Menu>(ID), "main", "start",
			[this]() {
			mChannel.broadcast(PlaySound("electro.wav"));

			SceneManager::set(SceneManager::State::active, SceneManager::State::sleep);
			mChannel.broadcast(LoadWorld("normal", SceneManager::addScene("game", SceneManager::State::active)));
		});

		MenuFactory::get().addAction(*container.getComponent<Menu>(ID), "main", "options_msg",
			[this]() {
			mChannel.broadcast(PlaySound("click.wav")); });

		MenuFactory::get().addAction(*container.getComponent<Menu>(ID), "options", "back_msg",
			[this]() {
			mChannel.broadcast(PlaySound("click.wav")); });

		MenuFactory::get().setActualScreen(*container.getComponent<Menu>(ID), "main");

	}

    void Game::createWinScreen(){
        int sceneID = SceneManager::addScene("win_scene", SceneManager::State::sleep);
		GuiStyle* gui;
		gui = Stylesheets.Get("text");
		gui->font = fonts.Get(gui->fontName);

		int ID = container.getUniqueID();
		container.createComponent<Transform>(ID);

		container.createComponent<Menu>(ID);

		container.createComponent<Scene>(ID);

		container.getComponent<Scene>(ID)->sceneID = sceneID;

		container.getComponent<Transform>(ID)->x = 100;
		container.getComponent<Transform>(ID)->y = views.VIEW_HEIGHT / 2.f;
		int xsize = 300;
		container.getComponent<Menu>(ID)->name = "win_menu";
		container.getComponent<Menu>(ID)->z = 10000;

		MenuFactory::get().addScreen(*container.getComponent<Menu>(ID), "main");
		MenuFactory::get().addGui(*
                            container.getComponent<Menu>(ID), "main", sf::Vector2f(0, 64.0 * 1), sf::Vector2f(xsize, 64), 4, false, *gui,
		{  std::make_pair("Next level", "next_msg"), std::make_pair("Quit", "quit_msg") });

		MenuFactory::get().addGui(*
                            container.getComponent<Menu>(ID), "main", sf::Vector2f(0, 600), sf::Vector2f(500, 256), 4, false, *gui,
		{ std::make_pair("U  RECT DEM"," die_msg") });
		/*MenuFactory::get().addGui(*
                            container.getComponent<Menu>(ID), "options", sf::Vector2f(0, 64.0 * 0.5), sf::Vector2f(xsize, 64), 4, false, *gui,
		{ std::make_pair("Back", "back_msg") });*/
		//MenuFactory::get().addConnection(*container.getComponent<Menu>(ID), "main", "options_msg", "options");
		//MenuFactory::get().addConnection(*container.getComponent<Menu>(ID), "options", "back_msg", "main");
		//MenuFactory::get().addConnection(*container.getComponent<Menu>(ID), "main", "start", "options");

		MenuFactory::get().addAction(*container.getComponent<Menu>(ID), "main", "quit_msg",
			[this]() {
			//mChannel.broadcast(PlaySound("click.wav"));
			//mChannel.broadcast(MenuEvent("0", MenuEvent::hide));
			mChannel.broadcast(Engine::StopEvent()); });

		MenuFactory::get().addAction(*container.getComponent<Menu>(ID), "main", "next_msg",
			[this]() {
			//mChannel.broadcast(PlaySound("electro.wav"));
            //SceneManager::modState("game",SceneManager::destroy);
			SceneManager::set(SceneManager::State::active, SceneManager::State::sleep);
			mChannel.broadcast(LoadWorld("next", SceneManager::addScene("game", SceneManager::State::active)));
		});

		MenuFactory::get().setActualScreen(*container.getComponent<Menu>(ID), "main");
    }

	void Game::createGameOverScreen(){
        int sceneID = SceneManager::addScene("game_over", SceneManager::State::sleep);
		GuiStyle* gui;
		gui = Stylesheets.Get("text");
		gui->font = fonts.Get(gui->fontName);

		int ID = container.getUniqueID();
		container.createComponent<Transform>(ID);

		container.createComponent<Menu>(ID);

		container.createComponent<Scene>(ID);

		container.getComponent<Scene>(ID)->sceneID = sceneID;

		container.getComponent<Transform>(ID)->x = 100;
		container.getComponent<Transform>(ID)->y = views.VIEW_HEIGHT / 2.f;
		int xsize = 300;
		container.getComponent<Menu>(ID)->name = "main";
		container.getComponent<Menu>(ID)->z = 10000;

		MenuFactory::get().addScreen(*container.getComponent<Menu>(ID), "main");
		MenuFactory::get().addGui(*
                            container.getComponent<Menu>(ID), "main", sf::Vector2f(0, 64.0 * 1.5), sf::Vector2f(xsize, 64), 4, false, *gui,
		{ std::make_pair("You  Died","die_msg"), std::make_pair("Restart", "restart_msg"), std::make_pair("Quit", "quit_msg") });
		/*MenuFactory::get().addGui(*
                            container.getComponent<Menu>(ID), "options", sf::Vector2f(0, 64.0 * 0.5), sf::Vector2f(xsize, 64), 4, false, *gui,
		{ std::make_pair("Back", "back_msg") });*/
		//MenuFactory::get().addConnection(*container.getComponent<Menu>(ID), "main", "options_msg", "options");
		//MenuFactory::get().addConnection(*container.getComponent<Menu>(ID), "options", "back_msg", "main");
		//MenuFactory::get().addConnection(*container.getComponent<Menu>(ID), "main", "start", "options");

		MenuFactory::get().addAction(*container.getComponent<Menu>(ID), "main", "quit_msg",
			[this]() {
			//mChannel.broadcast(PlaySound("click.wav"));
			//mChannel.broadcast(MenuEvent("0", MenuEvent::hide));
			mChannel.broadcast(Engine::StopEvent()); });

		MenuFactory::get().addAction(*container.getComponent<Menu>(ID), "main", "restart_msg",
			[this]() {
			//mChannel.broadcast(PlaySound("electro.wav"));
            SceneManager::modState("game",SceneManager::destroy);
			SceneManager::set(SceneManager::State::active, SceneManager::State::sleep);
			mChannel.broadcast(LoadWorld("normal", SceneManager::addScene("game", SceneManager::State::active)));
		});

		MenuFactory::get().setActualScreen(*container.getComponent<Menu>(ID), "main");
	}

	void Game::createWindowAndStuff() {
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
		mChannel.broadcast(Volume(xml.get<float>("config.volume:music"), xml.get<float>("config.volume:sound")));
        views.declareTextures(views.VIEW_WIDTH, views.VIEW_HEIGHT);
        engine.setVariables(&views, &window);
	}
