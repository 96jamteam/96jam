//
//  particle_system.h
//  maybe
//
//  Created by Janek on 01/05/16.
//  Copyright © 2016 Janek. All rights reserved.
//

#ifndef particle_system_h
#define particle_system_h

#include "system.h"
#include "particle_cmp.h"
#include <SFML/Graphics.hpp>

class ParticleSystem : public System{
    ComponentContainer* cc;
    componentContainer::container<Particle>* particles;
    sf::Clock clock;
    
public:
    ParticleSystem(ComponentContainer* _cc):cc(_cc){
        particles = cc->getComponentStorage<Particle>();
        if (particles == nullptr)
            particles = cc->addComponentStorage<Particle>();
        mChannel.add<AddParticle>(*this);
    }
    
    void update(){
        float dt = clock.restart().asSeconds();
        
        
        for(Particle p : *particles){
            p.lifespan -= dt;
            if (p.lifespan < 0){
                cc->deleteComponent<Particle>(p.entityID);
            }
        }
    }
    
    void handle(const AddParticle& e) {
        int entity = cc->getUniqueID();
        
        cc->createComponent<Scene>(entity);
        cc->getComponent<Scene>(entity)->sceneID = SceneManager::getState("game");
        cc->createComponent<ContactName>(entity);
        cc->getComponent<ContactName>(entity)->name = "particle";
        
        cc->createComponent<Transform>(entity);
        cc->getComponent<Transform>(entity)->x = e.x;
        cc->getComponent<Transform>(entity)->y = e.y;
        cc->getComponent<Transform>(entity)->angle =  0;
    }
};
#endif /* particle_system_h */
