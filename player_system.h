#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <algorithm>

#include "system.h"
#include "structures_for_broadcast.h"
#include "stuff.h"
#include "views.h"
#include "player_cmp.h"
#include "transform_cmp.h"
#include "weapon_cmp.h"
#include "game.h"

class PlayerSystem : public System {

    sf::RenderWindow* window;
    Views* views;
    Game* game;
    ComponentContainer* cc;
	componentContainer::container<Player>* players;
	Physics* physics;
    Transform* transform;
    AnimatedSpriteC* anim;
	WeaponC* weapon;
    sf::Clock clock;
    KeyboardManager km;
    bool shot;
public:


    PlayerSystem(sf::RenderWindow* t, ComponentContainer* _cc, Game* _game, Views* _views) :
    window(t), game(_game), cc(_cc), views(_views){
        players = cc->getComponentStorage<Player>();
        if (players == nullptr)
            players = cc->addComponentStorage<Player>();
        mChannel.add<PhysicsUpdate>(*this);
        mChannel.add<KillPlayer>(*this);
        mChannel.add<ControlsUpdate>(*this);
        km.load("controls.txt");
        shot=0;
    }

    virtual ~PlayerSystem()
    {

    }

    virtual void update() {

		for (int i = 0; i < players->size(); i++) {
			if (!(*players)[i].active || !(*players)[i].alive)
				continue;

			transform = cc->getComponent<Transform>((*players)[i].entityID);


			if (transform != nullptr) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !shot ){
					shoot(i);
					shot=1;
				}
				if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    shot=0;

			}
		}
    }


    void shoot(int i){
		weapon = cc->getComponent<WeaponC>((*players)[i].entityID);
		if (weapon != nullptr) {
			if (weapon->active_weapon >= 0) {
				sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
				sf::Vector2f worldPos = window->mapPixelToCoords(pixelPos, views->gameView);
				sf::Vector2f normalized = stuff::norm_vec(sf::Vector2f(worldPos.x - transform->x, worldPos.y - transform->y));
				mChannel.broadcast(ShootBullet(&weapon->weapons[weapon->active_weapon], transform->x, transform->y, normalized, cc->getComponent<Scene>((*players)[i].entityID)->sceneID ));
			}
/**<  */		}
    }

    void handle(const KillPlayer& e) {
        cc->getComponent<Player>(e.entityid)->alive=0;
    }
    void handle(const ControlsUpdate& e) {
        km.load("controls.txt");
    }

	void handle(const PhysicsUpdate& e) {
		game->views.clearTargets();
		for (int i = 0; i < players->size(); i++) {

			if (!(*players)[i].active || !(*players)[i].alive)
				continue;

			float pnm100 = 100 * ((*players)[i].number);

            //P(km.isDown("right"));
            float xb = km.isDown("right") - km.isDown("left");
			float yb = km.isDown("down") - km.isDown("up");

			sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
            sf::Vector2f worldPos = window->mapPixelToCoords(pixelPos, views->gameView);

            float p_x, p_y;
            getTrans(i, p_x, p_y);

			sf::Vector2f vel = sf::Vector2f(worldPos.x-p_x, worldPos.y - p_y);
            stuff::norm_vec(vel);

            float lookingAngle = atan(vel.y/vel.x)-stuff::PI/2 - (vel.x > 0.f ? stuff::PI : 0.f);


            physics = cc->getComponent<Physics>((*players)[i].entityID);
            transform = cc->getComponent<Transform>((*players)[i].entityID);
            anim = cc->getComponent<AnimatedSpriteC>((*players)[i].entityID);

			if (physics != nullptr) {
            physics->body->SetTransform(b2Vec2(p_x/stuff::SCALE, p_y/stuff::SCALE), lookingAngle);

				if (xb > 0.1 || xb < -0.1 || yb > 0.1 || yb < -0.1) {
					xb *= e.timestep;
					yb *= e.timestep;

                    mChannel.broadcast(new AddParticle(transform->x, transform->y));

                    #ifdef __APPLE__
					physics->body->ApplyForce(b2Vec2(xb*(*players)[i].speed*physics->body->GetMass(), yb*(*players)[i].speed*physics->body->GetMass()), physics->body->GetWorldCenter(), 1);
                    #else
                    physics->body->ApplyForce(b2Vec2(xb*(*players)[i].speed*physics->body->GetMass(), yb*(*players)[i].speed*physics->body->GetMass()), physics->body->GetWorldCenter());
                    #endif
                    /*if(!anim->sprites[0].isPlaying()){
                        if(anim->sprites[0].m_animation == game->Animations.getAnimation("playershooting")){
                            anim->sprites[0].play(*game->Animations.getAnimation("playerwalking"));
                            anim->sprites[0].setLooped(true);
                        }
                    }*/
                    if(!anim->sprites[0].isPlaying()){
                        anim->sprites[0].play();
                    }
                }else{
                    anim->sprites[0].pause();
                }
				game->views.addTarget(sf::Vector2f(physics->body->GetPosition().x*stuff::SCALE, physics->body->GetPosition().y*stuff::SCALE));

			}

		}
		game->views.setCenter();
		window->setView(views->gameView);
	}


	bool getTrans(const int& i, float& x, float& y){
        if(!(*players)[i].active)
            return false;
        transform = cc->getComponent<Transform>((*players)[i].entityID);
            if (transform != nullptr) {
                x=transform->x;
                y=transform->y;
                return true;
            }
        return false;
    }
};
