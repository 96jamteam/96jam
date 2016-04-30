#pragma once

#include<iostream>
#include<SFML/Graphics.hpp>

#include "system.h"
#include"menu_cmp.h"
#include"transform_cmp.h"
#include"stuff.h"
#include "structures_for_broadcast.h"

class MenuSystem :
	public System
{
	sf::RenderTarget* target;
	ComponentContainer* cc;
	componentContainer::container<Menu>* components;
	sf::Vector2f mousePos;
	bool pressed;
public:
	MenuSystem(sf::RenderTarget* t, ComponentContainer* _cc) :
		target(t), cc(_cc) {
		//enableUpdater(1, OrderBitset(), ResourceBitset(), Task::NONE);
        components = cc->getComponentStorage<Menu>();
        if (components == nullptr)
            components = cc->addComponentStorage<Menu>();

        mChannel.add<MousePos>(*this);
        mChannel.add<MouseButtonEvent>(*this);
        mChannel.add<MenuEvent>(*this);
        mChannel.add<SetMenuScreenEvent>(*this);
	}

	virtual ~MenuSystem() {

	}

	void handle(const MousePos& mpos){

		mousePos.x = mpos.x;
		mousePos.y = mpos.y;

		for (int j = 0; j < components->size(); j++) {

			if((*components)[j].active)
				for (int i = 0; i< ((*components)[j]).screens.at(((*components)[j]).actualScreen).guiSystem.size(); i++)
					(*components)[j].screens.at((*components)[j].actualScreen).guiSystem[i].highlight((*components)[j].screens.at((*components)[j].actualScreen).guiSystem[i].getEntry(mousePos));
		}

	}

	void handle(const MouseButtonEvent& mbevn){
		if (mbevn.button == sf::Mouse::Button::Left ) {

			pressed = mbevn.pressed;

			for (int j = 0; j < components->size(); j++) {

				if ((*components)[j].active)
					if (pressed){
						for (int i = 0; i<  (*components)[j] .screens.at((*components)[j] .actualScreen).guiSystem.size(); i++){
							std::string msg = (*components)[j] .screens.at((*components)[j] .actualScreen).guiSystem[i].activate(mousePos);

							//aktywuj akcjê
							if ((*components)[j] .screens.at((*components)[j] .actualScreen).actions.find(msg) != (*components)[j] .screens.at((*components)[j] .actualScreen).actions.end())
								(*components)[j] .screens.at((*components)[j] .actualScreen).actions.at(msg)();

							//przejdŸ do ekranu
							if ((*components)[j] .screens.at((*components)[j] .actualScreen).connections.find(msg) != (*components)[j] .screens.at((*components)[j] .actualScreen).connections.end())
								(*components)[j] .actualScreen = (*components)[j] .screens.at( (*components)[j] .actualScreen).connections.at(msg);
						}
					}
			}
		}
	}

	void handle(const MenuEvent& menu_event) {
		for (int j = 0; j < components->size(); j++) {

			if ((*components)[j].name==menu_event.menu_name)
				switch (menu_event.type)
				{
				case MenuEvent::hide:
					(*components)[j].active = 0;
					break;
				case MenuEvent::show:
					(*components)[j].active = 1;
					break;
				default:
					break;
				}

		}
	}

	void handle(const SetMenuScreenEvent& menu_event) {
		for (int j = 0; j < components->size(); j++) {
			if ((*components)[j].name == menu_event.menu_name)
				(*components)[j].actualScreen = menu_event.set_screen;
		}
	}

	virtual void update() {

	}


};

