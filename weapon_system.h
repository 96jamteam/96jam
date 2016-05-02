#pragma once
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
#include "callback_cmp.h"
#include "bot_cmp.h"

class WeaponSystem : public System {

	ComponentContainer* cc;
	componentContainer::container<WeaponC>* weapons;
	b2World* world;
	sf::Clock clock;
	float factor = 1.f;
public:
	WeaponSystem(ComponentContainer* _cc, b2World* _world) : cc(_cc), world(_world){

		weapons = cc->getComponentStorage<WeaponC>();
		if (weapons == nullptr)
			weapons = cc->addComponentStorage<WeaponC>();
			mChannel.add<BulletTime>(*this);
		mChannel.add<ShootBullet>(*this);
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
                if(!chceckVis( b2Vec2(sb.x / stuff::SCALE, sb.y / stuff::SCALE),
                             rotatePoint(sb.x / stuff::SCALE, sb.y / stuff::SCALE, atan2(sb.norm_vec.y , sb.norm_vec.x),
                                          b2Vec2((sb.x + sb.weapon->spawnPoint.x) / stuff::SCALE, (sb.y+ sb.weapon->spawnPoint.y) / stuff::SCALE) ),world,true )){
                    return;
                }
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

            cc->createComponent<CallbackCmp>(entityID);
            cc->getComponent<CallbackCmp>(entityID)->callbacks["bullet_begin"]=([this, entityID](){
                EventChannel chan;
                chan.broadcast(DeleteThisShit(entityID));
            });



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

	bool chceckVis(const b2Vec2& pos1, const b2Vec2& pos2, b2World* world, bool dynamic, bool threeWay=0)
    {
    using namespace stuff;
    bool hit(0);
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
        for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
        {
            if(f->IsSensor())
                continue;
            if(b->GetType()==b2_dynamicBody && !dynamic)
                break;
            if(cc->getComponent<Bot>((intptr_t)b->GetUserData())!=nullptr)
                break;

            if(cc->getComponent<Player>((intptr_t)b->GetUserData())!=nullptr)
                break;

            b2RayCastOutput output;
            b2RayCastInput input;
            input.p1=pos1;
            input.p2 =pos2;
            input.maxFraction = 1;
#ifdef __APPLE__
            if ( f->RayCast( &output, input,0) )
            {
                hit=true;
            }
            if(threeWay)
            {
                float wide=25.f;
                double ang1=atan2(pos2.y -pos1.y, pos2.x-pos1.x);
                b2Vec2 one,two;
                one=b2Vec2(pos1.x + (wide/SCALE)*cos(ang1-degtorad(90)), pos1.y + (wide/SCALE)*sin(ang1-degtorad(90)));
                two=b2Vec2(pos2.x + (wide/SCALE)*cos(ang1-degtorad(90)), pos2.y + (wide/SCALE)*sin(ang1-degtorad(90)));
                input.p1=one;
                input.p2 =two;
                input.maxFraction = 1;

                if ( f->RayCast( &output, input,0) )
                {
                    hit=true;
                }

                one=b2Vec2(pos1.x + (wide/SCALE)*cos(ang1+degtorad(90)), pos1.y + (wide/SCALE)*sin(ang1+degtorad(90)));
                two=b2Vec2(pos2.x + (wide/SCALE)*cos(ang1+degtorad(90)), pos2.y + (wide/SCALE)*sin(ang1+degtorad(90)));
                input.p1=one;
                input.p2 =two;
                input.maxFraction = 1;

                if ( f->RayCast( &output, input ,0) )
                {
                    hit=true;
                }
            }
#else
            if ( f->RayCast( &output, input) )
            {
                hit=true;
            }
            if(threeWay)
            {
                float wide=25.f;
                double ang1=atan2(pos2.y -pos1.y, pos2.x-pos1.x);
                b2Vec2 one,two;
                one=b2Vec2(pos1.x + (wide/SCALE)*cos(ang1-degtorad(90)), pos1.y + (wide/SCALE)*sin(ang1-degtorad(90)));
                two=b2Vec2(pos2.x + (wide/SCALE)*cos(ang1-degtorad(90)), pos2.y + (wide/SCALE)*sin(ang1-degtorad(90)));
                input.p1=one;
                input.p2 =two;
                input.maxFraction = 1;

                if ( f->RayCast( &output, input) )
                {
                    hit=true;
                }

                one=b2Vec2(pos1.x + (wide/SCALE)*cos(ang1+degtorad(90)), pos1.y + (wide/SCALE)*sin(ang1+degtorad(90)));
                two=b2Vec2(pos2.x + (wide/SCALE)*cos(ang1+degtorad(90)), pos2.y + (wide/SCALE)*sin(ang1+degtorad(90)));
                input.p1=one;
                input.p2 =two;
                input.maxFraction = 1;

                if ( f->RayCast( &output, input ) )
                {
                    hit=true;
                }
            }
#endif
        }
    return !hit;
    }
};
