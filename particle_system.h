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
#include "particle_creator.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

class ParticleSystem : public System{
    ComponentContainer* cc;
    sf::Clock clock;
    componentContainer::container<Particle>* particles;
    componentContainer::container<ParticleEmitter>* emitters;

public:
    ParticleSystem(ComponentContainer* _cc):cc(_cc){
        particles = cc->getComponentStorage<Particle>();
        if (particles == nullptr)
            particles = cc->addComponentStorage<Particle>();

        emitters = cc->getComponentStorage<ParticleEmitter>();
        if (emitters == nullptr)
            emitters = cc->addComponentStorage<ParticleEmitter>();

        //mChannel.add<AddParticle>(*this);
    }

    void update()
    {
        float dt = clock.restart().asSeconds();

        for (int i = 0; i < particles->size(); i++) {
			if (!(*particles)[i].active)
				continue;
            (*particles)[i].lifespan -= dt;
            if ((*particles)[i].lifespan < 0)
            {
                mChannel.broadcast(DeleteThisShit((*particles)[i].entityID));
            }

        }


        for (int i = 0; i < emitters->size(); i++) {
			if (!(*emitters)[i].active)
				continue;

            (*emitters)[i].emitspan -= dt;
            if ((*emitters)[i].emitspan < 0)
            {

                (*emitters)[i].emitspan+=(*emitters)[i].timetoemit;
                int entityID = ParticleCreator::createParticle((*emitters)[i].formula);

                Transform* particle_tran=cc->getComponent<Transform>(entityID);
                Transform* emitter_tran=cc->getComponent<Transform>((*emitters)[i].entityID);

                if(particle_tran!=nullptr && emitter_tran!=nullptr){
                    particle_tran->x=emitter_tran->x;//+stuff::random(-5.f,5.f);
                    particle_tran->y=emitter_tran->y;//+stuff::random(-5.f,5.f);
                    particle_tran->angle=emitter_tran->angle;
                }
            }

        }
    }

    /*void handle(const AddParticle& e) {

        int entity = cc->getUniqueID();
        cc->createComponent<Scene>(entity); //Stanisz// 'chyba rozumiem ten syf':>
        cc->getComponent<Scene>(entity)->sceneID = SceneManager::getState("game");
        cc->createComponent<ContactName>(entity);
        cc->getComponent<ContactName>(entity)->name = "particle";
        cc->createComponent<Transform>(entity);
        cc->getComponent<Transform>(entity)->x = e.x;
        cc->getComponent<Transform>(entity)->y = e.y;
        cc->getComponent<Transform>(entity)->angle =  0;

    }*/
};
#endif /* particle_system_h */
