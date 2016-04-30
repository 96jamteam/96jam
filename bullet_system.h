#pragma once
#include<iostream>
#include<SFML/Graphics.hpp>

#include"system.h"
#include "structures_for_broadcast.h"
#include "stuff.h"
#include "views.h"
#include "player_cmp.h"
#include "transform_cmp.h"
#include "weapon_cmp.h"
#include "sprite_cmp.h"
#include "bullet_cmp.h"
#include "game.h"

class BulletSystem : public System {
	ComponentContainer* cc;
	componentContainer::container<Bullet>* bullets;

	b2World* world;
	sf::Clock clock;
	float factor = 1.f;
public:
	BulletSystem(ComponentContainer* _cc, b2World* _world) :
		System("BulletSystem"), cc(_cc), world(_world) {
		int id = BitsetBuilder::getOrderID("BulletSystem");
		BitsetBuilder::addAllResources("BulletSystem");
		enableUpdater(id, ChiefInfo::Container<int>(), BitsetBuilder::getResources("BulletSystem"), Task::SINGLETHREADED_REPEATING);
		bullets = cc->getComponentStorage<Bullet>();
		if (bullets == nullptr)
			bullets = cc->addComponentStorage<Bullet>();
	}

	virtual ~BulletSystem()
	{

	}

	virtual bool init() {
		mChannel.broadcast(SystemInitializing(this));
		mChannel.add<BulletTime>(*this);

		return true;
	}

	virtual void update() {
		float dt = clock.restart().asSeconds() * factor;
		for (int i = 0; i < bullets->size(); i++) {
			if ((*bullets)[i].active==true){
				if ((*bullets)[i].lifetime - dt>0) {
					(*bullets)[i].lifetime -= dt;
				}
				else {
					b2Body* body = cc->getComponent<Physics>((*bullets)[i].entityID)->body;
					if (body != nullptr)
						body->GetWorld()->DestroyBody(body);
					cc->deleteComponents((*bullets)[i].entityID);
					mChannel.broadcast(SpriteAdded());
				}
			}

		}
	}

	void handle(const BulletTime& btime) {
		factor = btime.factor;
	}

};