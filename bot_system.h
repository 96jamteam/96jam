
#ifndef bot_system_h
#define bot_system_h

#include <stdio.h>
#include"system.h"
#include"engine.h"
#include "bot_cmp.h"
#include "player_cmp.h"
#include"game.h"

class BotSystem : public System{

    ComponentContainer* cc;
    componentContainer::container<Bot>* bots;
    componentContainer::container<Player>* players;
    Physics* physics;
    b2World* world;
    Transform* transform;

    bool getTrans(const int& i, float& x, float& y){
        if(!(*bots)[i].active)
            return false;
        transform = cc->getComponent<Transform>((*bots)[i].entityID);
            if (transform != nullptr) {
                x=transform->x;
                y=transform->y;
                return true;
            }
        return false;
    }

public:
    BotSystem(ComponentContainer* _cc,b2World* _world):cc(_cc),world(_world){
        bots = cc->getComponentStorage<Bot>();
        if (bots == nullptr)
            bots = cc->addComponentStorage<Bot>();
        players = cc->getComponentStorage<Player>();
        if (players == nullptr)
            players = cc->addComponentStorage<Player>();
        mChannel.add<PhysicsUpdate>(*this);
    }

    void update(){

    }

    void handle(const PhysicsUpdate& e) {

        float b_x, b_y, p_x, p_y;
        for(unsigned int i = 0;i < players->size();i++){
            if(!(*players)[i].active)
                continue;
            transform = cc->getComponent<Transform>((*players)[i].entityID);
            if (transform != nullptr) {
                p_x=transform->x;
                p_y=transform->y;
            }
        }

        for (int i = 0; i < bots->size(); i++) {

            if (!(*bots)[i].active)
                continue;

        transform = cc->getComponent<Transform>((*bots)[i].entityID);
            if (transform != nullptr) {
                b_x=transform->x;
                b_y=transform->y;
            }


        (*bots)[i].isLooking = false;
        if (chceckVis(b2Vec2(p_x/stuff::SCALE,p_y/stuff::SCALE), b2Vec2(b_x/stuff::SCALE,b_y/stuff::SCALE),world,true))
        {
            (*bots)[i].isLooking = true;
            (*bots)[i].target.x = p_x;
            (*bots)[i].target.y = p_y;

        }
        else
        {
            float seeX, seeY;

            if((*bots)[i].heSees != 2147483647)
            {
            getTrans((*bots)[i].heSees, seeX, seeY);

            if(chceckVis(b2Vec2(seeX/stuff::SCALE,seeY/stuff::SCALE), b2Vec2(b_x/stuff::SCALE,b_y/stuff::SCALE),world,true))
            {
                (*bots)[i].isLooking = 1;
                (*bots)[i].target.x = seeX;
                (*bots)[i].target.y = seeY;
            }
            else
            {
                (*bots)[i].heSees = 2147483647;

                for(unsigned int b = 0;b < bots->size();b++)
                {
                    getTrans(b, seeX, seeY);

                    if(b != i && (*bots)[b].isLooking && chceckVis(b2Vec2(seeX/stuff::SCALE,seeY/stuff::SCALE), b2Vec2(b_x/stuff::SCALE,b_y/stuff::SCALE),world,true))
                    {
                        (*bots)[i].isLooking=1;

                        (*bots)[i].heSees = b;

                        (*bots)[i].target.x = seeX;
                        (*bots)[i].target.y = seeY;
                    }
                }

            }

            }

        }


        sf::Vector2f vel;



            vel = sf::Vector2f((*bots)[i].target.x-b_x,(*bots)[i].target.y-b_y);
            stuff::norm_vec(vel);

            if((*bots)[i].target.x == -69.6969696969f)
            {
                (*bots)[i].freeWalkingAngle += float(rand()%100-50)/100.f* 1.f;
                //(*bots)[i].freeWalkingAngle += 1.0f;
                vel = sf::Vector2f((float)sin((*bots)[i].freeWalkingAngle), (float)cos((*bots)[i].freeWalkingAngle));
                stuff::norm_vec(vel);

                vel.x *= 40;
                vel.y *= 40;
            }

            float lookingAngle = atan(vel.y/vel.x)-stuff::PI/2 - (vel.x > 0.f ? stuff::PI : 0.f);


            float xb = vel.x * (*bots)[i].speed;
            float yb = vel.y * (*bots)[i].speed;



            physics = cc->getComponent<Physics>((*bots)[i].entityID);



            if (physics != nullptr) {
physics->body->SetTransform( physics->body->GetPosition(), lookingAngle);
                if (xb > 0.1 || xb < -0.1 || yb > 0.1 || yb < -0.1) {
                    xb *= e.timestep;
                    yb *= e.timestep;

                    physics->body->SetTransform( physics->body->GetPosition(), lookingAngle);

                    #ifdef __APPLE__
                    physics->body->ApplyForce(b2Vec2(xb*(*bots)[i].speed*physics->body->GetMass(), yb*(*bots)[i].speed*physics->body->GetMass()), physics->body->GetWorldCenter(), 1);
                    #else
                    physics->body->ApplyForce(b2Vec2(xb*(*bots)[i].speed*physics->body->GetMass(), yb*(*bots)[i].speed*physics->body->GetMass()), physics->body->GetWorldCenter());
                    #endif
                }
            }

        }
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

#endif /* bot_system_h */
