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
	WeaponC* weapon;
    sf::Clock clock;
public:


    PlayerSystem(sf::RenderWindow* t, ComponentContainer* _cc, Game* _game, Views* _views) :
    window(t), game(_game), cc(_cc), views(_views){
        players = cc->getComponentStorage<Player>();
        if (players == nullptr)
            players = cc->addComponentStorage<Player>();
        mChannel.add<PhysicsUpdate>(*this);
    }

    virtual ~PlayerSystem()
    {

    }

    virtual void update() {

		for (int i = 0; i < players->size(); i++) {
			if (!(*players)[i].active)
				continue;

			transform = cc->getComponent<Transform>((*players)[i].entityID);

			if (transform != nullptr) {
				//if (InputManager::action( 100 * ((*players)[i].number) + InputManager::actionKeys::shoot ) ){
				//	shoot(i);
				//}

			}
		}
    }


    void shoot(int i){
		weapon = cc->getComponent<WeaponC>((*players)[i].entityID);
		if (weapon != nullptr) {
			if (weapon->active_weapon >= 0) {
				sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
				sf::Vector2f worldPos = window->mapPixelToCoords(pixelPos, views->gameView);
				sf::Vector2f normalized;
				/*if (InputManager::action(100 * ((*players)[i].number) + InputManager::actionKeys::mouseUse))
					normalized = stuff::norm_vec(sf::Vector2f(worldPos.x - transform->x, worldPos.y - transform->y));
				else {
					float right = InputManager::action(100 * ((*players)[i].number) + InputManager::actionKeys::c_right);
					float left = InputManager::action(100 * ((*players)[i].number) + InputManager::actionKeys::c_left);
					float up = InputManager::action(100 * ((*players)[i].number) + InputManager::actionKeys::c_up);
					float down = InputManager::action(100 * ((*players)[i].number) + InputManager::actionKeys::c_down);
					normalized = stuff::norm_vec(sf::Vector2f(right-left,down-up));
					if (!normalized.x && !normalized.y)
						normalized = stuff::norm_vec(sf::Vector2f(0,1));
				}*/
				mChannel.broadcast(ShootBullet(&weapon->weapons[weapon->active_weapon], transform->x, transform->y, normalized, cc->getComponent<Scene>((*players)[i].entityID)->sceneID ));
			}
		}
    }

	void handle(const PhysicsUpdate& e) {
		game->views.clearTargets();
		for (int i = 0; i < players->size(); i++) {

			if (!(*players)[i].active)
				continue;

			float pnm100 = 100 * ((*players)[i].number);

			//float xb = InputManager::action(pnm100 + InputManager::actionKeys::m_right) - InputManager::action(pnm100 + InputManager::actionKeys::m_left);
			//float yb = InputManager::action(pnm100 + InputManager::actionKeys::m_down) - InputManager::action(pnm100 + InputManager::actionKeys::m_up);

			physics = cc->getComponent<Physics>((*players)[i].entityID);
			if (physics != nullptr) {

				//if (xb > 0.1 || xb < -0.1 || yb > 0.1 || yb < -0.1) {
				//	xb *= e.timestep;
				//	yb *= e.timestep;
				//	physics->body->ApplyForce(b2Vec2(xb*(*players)[i].speed*physics->body->GetMass(), yb*(*players)[i].speed*physics->body->GetMass()), physics->body->GetWorldCenter(), true);
				//}
				game->views.addTarget(sf::Vector2f(physics->body->GetPosition().x*stuff::SCALE, physics->body->GetPosition().y*stuff::SCALE));

			}

		}
		game->views.setCenter();
		window->setView(views->gameView);
	}
};
