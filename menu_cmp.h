#pragma once

#include"component.h"

#include"gui.h"

class Menu : public Component
{
public:
	struct Node
	{
		//po nacisniecu przycisku o nazwie string nastepuje przejscie do Ekranu string
		std::map<std::string, std::string> connections;

		std::vector<Gui> guiSystem;

		//po nacisnieciu przycisku odbierana jest wiadomosc, do niej jest przypisana funkcja w mapie actions
		std::map<std::string, std::function<void()> > actions;

	};

	//lista ekranow np. glowne menu, opcje, wybor gry itd
	std::map<std::string, Node> screens;

	//aktualny ekran
	std::string actualScreen;

	//nazwa menu
	std::string name;

    float z;
};

class MenuFactory
{
public:
	void addScreen(Menu& menu, std::string name){
		menu.screens.insert(std::make_pair(name, Menu::Node()));
	}

	void addGui(Menu& menu, std::string screenName, sf::Vector2f origin, sf::Vector2f dimensions, int padding, bool horizontal,
		GuiStyle& style, std::vector<std::pair<std::string, std::string>> entries){
		menu.screens[screenName].guiSystem.push_back(Gui(dimensions, padding, horizontal, style, entries));
		menu.screens[screenName].guiSystem[menu.screens[screenName].guiSystem.size() - 1].setPosition(origin);
		menu.screens[screenName].guiSystem[menu.screens[screenName].guiSystem.size() - 1].origin = origin;
		menu.screens[screenName].guiSystem[menu.screens[screenName].guiSystem.size() - 1].show();
	}

	void addGui(Menu& menu, std::string screenName, sf::Vector2f origin, Gui gui){
		menu.screens[screenName].guiSystem.push_back(gui);
		menu.screens[screenName].guiSystem[menu.screens[screenName].guiSystem.size() - 1].setPosition(origin);
		//menu.screens[screenName].origin = origin;
        menu.screens[screenName].guiSystem[menu.screens[screenName].guiSystem.size() - 1].origin = origin;
		menu.screens[screenName].guiSystem[menu.screens[screenName].guiSystem.size() - 1].show();
	}

	void addConnection(Menu& menu, std::string actualScreenName, std::string guiMessage, std::string nextScreenName){
		menu.screens[actualScreenName].connections[guiMessage] = nextScreenName;
	}

	void addAction(Menu& menu, std::string screenName, std::string guiMessage, std::function<void()> action){
		menu.screens[screenName].actions[guiMessage] = action;
	}

	void setActualScreen(Menu& menu, std::string name)
	{
		menu.actualScreen = name;
	}

	static MenuFactory& get(){
		static MenuFactory instance;
		return instance;
	}
private:
	MenuFactory() {}
};
