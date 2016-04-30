/*#pragma once
#include<iostream>
#include<SFML/Graphics.hpp>
#include <algorithm>

#include"system.h"
#include "structures_for_broadcast.h"
#include "stuff.h"
#include "views.h"
#include "player_cmp.h"
#include "transform_cmp.h"
#include "weapon_cmp.h"
#include "sprite_cmp.h"
#include "bullet_cmp.h"
#include "scene_cmp.h"
#include "animatedsprite_cmp.h"
#include "game.h"
#include "scene_manager.h"
#include "contact_name_cmp.h"

class WeaponSystem : public System {

	ComponentContainer* cc;
	componentContainer::container<WeaponC>* weapons;
	b2World* world;
	sf::Clock clock;
	float factor = 1.f;
public:
	WeaponSystem(ComponentContainer* _cc, b2World* _world) :
		System("WeaponSystem"), cc(_cc), world(_world){
		int id = BitsetBuilder::getOrderID("WeaponSystem");
		BitsetBuilder::addResource("WeaponSystem","WeaponC");
		enableUpdater(id, ChiefInfo::Container<int>(), BitsetBuilder::getResources("WeaponSystem"), Task::BACKGROUND_SYNC_REPEATING);
		weapons = cc->getComponentStorage<WeaponC>();
		if (weapons == nullptr)
			weapons = cc->addComponentStorage<WeaponC>();
	}

	virtual ~WeaponSystem()
	{

	}

	virtual bool init() {
		mChannel.broadcast(SystemInitializing(this));
		mChannel.add<BulletTime>(*this);
		mChannel.add<ShootBullet>(*this);
		return true;
	}

	virtual void update() {
		float dt = clock.restart().asSeconds() * factor;

		for (int i = 0; i < weapons->size(); i++) {
			if (!(*weapons)[i].active)
				continue;

			// cooldown broni
			if((*weapons)[i].active_weapon!=-1)
				(*weapons)[i].weapons[(*weapons)[i].active_weapon].cooldown -= dt;
		}
	}

	void handle(const BulletTime& btime) {
		factor = btime.factor;
	}
	void handle(const ShootBullet& sb) {
		if (sb.weapon->cooldown <= 0) {
			sb.weapon->cooldown = sb.weapon->maxCooldown;
			int entityID = cc->getUniqueID();
			cc->createComponent<Physics>(entityID);
			cc->createComponent<Transform>(entityID);
			cc->createComponent<Bullet>(entityID);
            cc->createComponent<Scene>(entityID);
            cc->createComponent<ContactName>(entityID);
            cc->getComponent<ContactName>(entityID)->name = "bullet";

			cc->getComponent<Scene>(entityID)->sceneID = sb.sceneID;

			//cc->getComponent<Bullet>(entityID)->lifetime = ((rand() % 1000) + 100) / 700.f; 6-7k
			cc->getComponent<Bullet>(entityID)->lifetime = 2;

			sb.weapon->bulletBodyDef.position = rotatePoint(sb.x / stuff::SCALE, sb.y / stuff::SCALE, atan2(sb.norm_vec.y , sb.norm_vec.x), b2Vec2((sb.x + sb.weapon->spawnPoint.x) / stuff::SCALE, (sb.y+ sb.weapon->spawnPoint.y) / stuff::SCALE) );

			float aimAngle = atan2(sb.norm_vec.y , sb.norm_vec.x ) + stuff::degtorad(stuff::random(-sb.weapon->spread, sb.weapon->spread));
			sb.weapon->bulletBodyDef.linearVelocity.x = cos(aimAngle) * sb.weapon->speed / stuff::SCALE;
			sb.weapon->bulletBodyDef.linearVelocity.y = sin(aimAngle) * sb.weapon->speed / stuff::SCALE;
			//sb.weapon->bulletBodyDef.linearVelocity = b2Vec2(sb.norm_vec.x * sb.weapon->speed / stuff::SCALE , sb.norm_vec.y * sb.weapon->speed / stuff::SCALE);

            sb.weapon->bulletBodyDef.userData = (void*) cc->getComponent<Physics>(entityID)->entityID;

			cc->getComponent<Physics>(entityID)->body = world->CreateBody(&sb.weapon->bulletBodyDef);
			cc->getComponent<Physics>(entityID)->body->CreateFixture(&sb.weapon->bulletFixDef);

			if (sb.weapon->bulletGraphics.graphicsType) {
				cc->createComponent<SpriteC>(entityID);
				cc->getComponent<SpriteC>(entityID)->sprites.push_back(sb.weapon->bulletGraphics.sprite);
				cc->getComponent<SpriteC>(entityID)->z.push_back(20);
				mChannel.broadcast(SpriteAdded());
			}
			else {
				cc->createComponent<AnimatedSpriteC>(entityID);
				cc->getComponent<AnimatedSpriteC>(entityID)->sprites.push_back(sb.weapon->bulletGraphics.animation);
				cc->getComponent<AnimatedSpriteC>(entityID)->z.push_back(20);
				mChannel.broadcast(SpriteAdded());

			}
		}
	}

	b2Vec2 rotatePoint(float cx, float cy, float angle, b2Vec2 p)
	{
		float s = sin(angle);
		float c = cos(angle);

		// translate point back to origin:
		p.x -= cx;
		p.y -= cy;

		// rotate point
		float xnew = p.x * c - p.y * s;
		float ynew = p.x * s + p.y * c;

		// translate point back:
		p.x = xnew + cx;
		p.y = ynew + cy;
		return p;
	}
};*/
