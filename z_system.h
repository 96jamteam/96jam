#pragma once
#include<iostream>
#include<SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>

#include"system.h"
#include "structures_for_broadcast.h"
#include "stuff.h"
#include "animatedsprite_cmp.h"
#include "transform_cmp.h"
#include "sprite_cmp.h"
#include "views.h"
#include "particle_system.h"

//#include <Vector2.hpp>

struct z_struct{
    int comp;
    int spr;
    short type;
    z_struct(const int& _comp, const int& _spr, const short& _type) : comp(_comp), spr(_spr), type(_type){}
};

class ZSystem : public System {

    sf::RenderTarget* target;
	Views* views;

	ComponentContainer* cc;

	componentContainer::container<AnimatedSpriteC>* anims;
	componentContainer::container<SpriteC>* sprites;
	componentContainer::container<Menu>* menus;

    sf::RectangleShape rect;
    sf::Texture spotlight;
    sf::Sprite sprite;

    std::deque<z_struct> order;

    sf::Clock clock;
	float factor = 1.f;
	int guiIndex = 0;

	Transform* transform;

public:
    ZSystem(sf::RenderTarget* t, ComponentContainer* cmpContainer, Views* _views) :
     target(t), cc(cmpContainer), views(_views)
    {
        anims = cc->getComponentStorage<AnimatedSpriteC>();
        if (anims == nullptr)
            anims = cc->addComponentStorage<AnimatedSpriteC>();

        sprites = cc->getComponentStorage<SpriteC>();
        if (sprites == nullptr)
            sprites = cc->addComponentStorage<SpriteC>();

        menus = cc->getComponentStorage<Menu>();
        if (menus == nullptr)
            menus = cc->addComponentStorage<Menu>();

        mChannel.add<SpriteAdded>(*this);
        mChannel.add<BulletTime>(*this);

        sf::Image spotImage;
        spotImage.create(views->VIEW_WIDTH, views->VIEW_HEIGHT);

        float rad = 400.0f;
        float gradientSize = 180.f;

        float light_constant = 2.0f;
        float light_linear=0.009;
        float light_quadratic=0.000032f;

        for (int i=0; i<views->VIEW_WIDTH; i++)
        {
            for (int j=0; j<views->VIEW_HEIGHT; j++)
            {
                spotImage.setPixel(i,j, sf::Color(0,0,0,1));

                float dst = stuff::distance(i,j,views->VIEW_WIDTH/2.f,views->VIEW_HEIGHT/2.f);

                if (dst >= rad + gradientSize-2)
                {
                    spotImage.setPixel(i,j,sf::Color(0,0,0));
                }
                else if (dst <= rad - gradientSize)
                {
                    spotImage.setPixel(i,j, sf::Color(0,0,0,0));
                }
                else if (dst < rad+gradientSize-1)
                {
                    spotImage.setPixel(i,j, sf::Color(0,0,0, ((dst-rad-gradientSize)/(gradientSize*2) * 255.f)));
                }

            }
        }

        spotlight.loadFromImage(spotImage);
        rect = sf::RectangleShape(sf::Vector2f(views->VIEW_WIDTH, views->VIEW_HEIGHT));
        rect.setTexture(&spotlight);
        rect.setPosition(0,0);
        sprite.setTexture(spotlight);
        sprite.setOrigin(spotlight.getSize().x/2.f,spotlight.getSize().y/2.f);

        handle(SpriteAdded());
    }

    virtual ~ZSystem()
    {

    }


    virtual void update() {
        sf::Time dt = sf::Time(sf::seconds(clock.restart().asSeconds() * factor));

        for (int j = 0; j < sprites->size(); j++){
			if (!(*sprites)[j].active)
				continue;
			transform = cc->getComponent<Transform>((*sprites)[j].entityID);
            if (transform != nullptr) {
                for (int i = 0; i < (*sprites)[j].sprites.size(); i++){
                    (*sprites)[j].sprites[i].setPosition(sf::Vector2f(transform->x, transform->y));
                    (*sprites)[j].sprites[i].setRotation(stuff::radtodeg(transform->angle));
                }
            }
        }

        for (int j = 0; j < anims->size(); j++){
			if (!(*anims)[j].active)
				continue;
			transform = cc->getComponent<Transform>((*anims)[j].entityID);
            if (transform != nullptr) {
                for (int i = 0; i < (*anims)[j].sprites.size(); i++){
                    (*anims)[j].sprites[i].setPosition(sf::Vector2f(transform->x, transform->y));
                    (*anims)[j].sprites[i].setRotation(stuff::radtodeg(transform->angle));
                    (*anims)[j].sprites[i].update(dt);
                }
            }
        }

        for (int j = 0; j < menus->size(); j++){
            if ((*menus)[j].active)
                for (int i = 0; i < (*menus)[j].screens.at((*menus)[j].actualScreen).guiSystem.size(); i++) {
					transform = cc->getComponent<Transform>((*menus)[j].entityID);
                    if (transform != nullptr) {
                        (*menus)[j].screens.at((*menus)[j].actualScreen).guiSystem[i].setPosition(sf::Vector2f(transform->x- (*menus)[j].screens.at((*menus)[j].actualScreen).guiSystem[i].origin.x,
                                                                                                                transform->y - (*menus)[j].screens.at((*menus)[j].actualScreen).guiSystem[i].origin.y));
                        (*menus)[j].screens.at((*menus)[j].actualScreen).guiSystem[i].show();
                    }
                }

        }




		//wszystko jest dobrze, nic nie mow
		//P(guiIndex<<" "<< order.size())
		//P(mName << " gameView: " << &views::gameView);
        target->setView(views->gameView);
		//target->clear(sf::Color::Red);
        for (int i = 0; i < guiIndex; i++) {
            if (order[i].type == 0){
                if ((*anims)[order[i].comp].active){
                    target->draw((*anims)[order[i].comp].sprites[order[i].spr]);
                }
            }else if(order[i].type == 1){
                if ((*sprites)[order[i].comp].active){
                    target->draw((*sprites)[order[i].comp].sprites[order[i].spr]);
                }
            }else if(order[i].type == 2){
                if ((*menus)[order[i].comp].active)
                {
                    for (int j = 0; j < (*menus)[order[i].comp].screens.at((*menus)[order[i].comp].actualScreen).guiSystem.size(); j++) {
                        target->draw((*menus)[order[i].comp].screens.at((*menus)[order[i].comp].actualScreen).guiSystem[j], sf::RenderStates());
                    }
                }
            }
        }
        sprite.setPosition(views->gameView.getCenter());
        target->draw(sprite);
        target->setView(views->guiView);

		for (int i = guiIndex; i < order.size(); i++) {
			if (order[i].type == 0) {
				if ((*anims)[order[i].comp].active)
					target->draw((*anims)[order[i].comp].sprites[order[i].spr]);
			}
			else if (order[i].type == 1) {
                if ((*sprites)[order[i].comp].active){
					target->draw((*sprites)[order[i].comp].sprites[order[i].spr]);
                }
			}
			else if (order[i].type == 2) {
				if ((*menus)[order[i].comp].active) {
					for (int j = 0; j < (*menus)[order[i].comp].screens.at((*menus)[order[i].comp].actualScreen).guiSystem.size(); j++) {
						target->draw((*menus)[order[i].comp].screens.at((*menus)[order[i].comp].actualScreen).guiSystem[j], sf::RenderStates());
					}
				}
			}
		}

        views->gameTexture.display();
        views->guiTexture.display();

    }

	void handle(const BulletTime& btime){
		factor = btime.factor;
	}

    void handle(const SpriteAdded& s){
        order.clear();
		guiIndex = 0;
        for (int j = 0; j < sprites->size(); j++)
			for (int i = 0; i < (*sprites)[j].sprites.size(); i++) {
				order.push_back(z_struct(j, i, 1));
			}

        for (int j = 0; j < anims->size(); j++)
			for (int i = 0; i < (*anims)[j].sprites.size(); i++) {
				order.push_back(z_struct(j, i, 0));
			}

		for (int j = 0; j < menus->size(); j++) {
			order.push_back(z_struct(j, 0, 2));
		}


        std::sort(order.begin(), order.end(),
                  [this](const z_struct& i,const z_struct& j) -> bool {
                      if (i.type == 0){
                          if(j.type == 0)
                              return (*anims)[i.comp].z[i.spr] < (*anims)[j.comp].z[j.spr];
                          else if (j.type == 1)
                              return (*anims)[i.comp].z[i.spr] < (*sprites)[j.comp].z[j.spr];
                          else
                              return (*anims)[i.comp].z[i.spr] < (*menus)[j.comp].z;
                      }else if(i.type == 1){
                          if(j.type == 0)
                              return (*sprites)[i.comp].z[i.spr] < (*anims)[j.comp].z[j.spr];
                          else if(j.type == 1)
                              return (*sprites)[i.comp].z[i.spr] < (*sprites)[j.comp].z[j.spr];
                          else
                              return (*sprites)[i.comp].z[i.spr] < (*menus)[j.comp].z;
                      }else{
                          if(j.type == 0)
                              return (*menus)[i.comp].z < (*anims)[j.comp].z[j.spr];
                          else if(j.type == 1)
                              return (*menus)[i.comp].z < (*sprites)[j.comp].z[j.spr];
                          else
                              return (*menus)[i.comp].z < (*menus)[j.comp].z;
                      }
                  });

		//takie cos do views
		for (int i = 0; i < order.size(); i++) {
			if (order[i].type == 0) {
				if ((*anims)[order[i].comp].z[order[i].spr] < 1000)
					guiIndex = i+1;
			}
			else if (order[i].type == 1) {
				if ((*sprites)[order[i].comp].z[order[i].spr] < 1000)
					guiIndex = i+1;
			}
			else if (order[i].type == 2) {
				if ((*menus)[order[i].comp].z < 1000) {
					guiIndex = i+1;
				}
			}

		}
    }
};
