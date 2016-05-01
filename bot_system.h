//
//  bot_system.h
//  maybe
//
//  Created by Janek on 01/05/16.
//  Copyright © 2016 Janek. All rights reserved.
//

#ifndef bot_system_h
#define bot_system_h

#include <stdio.h>
#include"system.h"
#include"engine.h"
#include "bot_cmp.h"

#include"game.h"

class BotSystem : public System{
    
    ComponentContainer* cc;
    componentContainer::container<Bot>* bots;
    Physics* physics;
    Transform* transform;
    
public:
    BotSystem(ComponentContainer* _cc):cc(_cc){
        bots = cc->getComponentStorage<Bot>();
        if (bots == nullptr)
            bots = cc->addComponentStorage<Bot>();
        mChannel.add<PhysicsUpdate>(*this);
    }
    
    void update(){
        
    }
    
    void handle(const PhysicsUpdate& e) {
        for (int i = 0; i < bots->size(); i++) {
            
            if (!(*bots)[i].active)
                continue;
            
            
            //P(km.isDown("right"));
            float xb = -1;
            float yb = 0;
            
            physics = cc->getComponent<Physics>((*bots)[i].entityID);
            
            if (physics != nullptr) {
                
                if (xb > 0.1 || xb < -0.1 || yb > 0.1 || yb < -0.1) {
                    xb *= e.timestep;
                    yb *= e.timestep;
                    
                    #ifdef __APPLE__
                    physics->body->ApplyForce(b2Vec2(xb*(*bots)[i].speed*physics->body->GetMass(), yb*(*bots)[i].speed*physics->body->GetMass()), physics->body->GetWorldCenter(), 1);
                    #else
                    physics->body->ApplyForce(b2Vec2(xb*(*bots)[i].speed*physics->body->GetMass(), yb*(*bots)[i].speed*physics->body->GetMass()), physics->body->GetWorldCenter());
                    #endif
                }
            }
            
        }
    }
};

#endif /* bot_system_h */
