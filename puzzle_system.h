#ifndef PUZZLE_SYSTEM_H_INCLUDED
#define PUZZLE_SYSTEM_H_INCLUDED

#include<iostream>
#include<SFML/Graphics.hpp>

#include "system.h"
#include "structures_for_broadcast.h"
#include "stuff.h"
#include "puzzle_cmp.h"
#include "transform_cmp.h"

class PuzzleSystem : public System {

private:
    EventChannel channel;
	ComponentContainer* cc;
	componentContainer::container<Puzzle>* puzzles;
	bool nextload;

public:
	PuzzleSystem(ComponentContainer& _cc) : cc(&_cc) {
		puzzles = cc->getComponentStorage<Puzzle>();
		if (puzzles == nullptr)
			puzzles = cc->addComponentStorage<Puzzle>();
nextload=0;

	}



	virtual void update() {
	    int done=0,max=0;
	    for (int j = 0; j < puzzles->size(); j++) {
			if (!(*puzzles)[j].active)
                continue;
            if ((*puzzles)[j].done)
                done++;
            max++;
		}
        if(done==max && max){
            SceneManager::modState("game", SceneManager::State::destroy);
            SceneManager::modState("menu", SceneManager::State::active);
            mChannel.broadcast(SceneUpdate());

            //mChannel.broadcast(LoadWorld( "next",SceneManager::addScene("game", SceneManager::State::active) ));

        }
		/*Transform* transform;
		for (int j = 0; j < physicsComponents->size(); j++) {
			if ((*physicsComponents)[j].active) {
				transform = cc->getComponent<Transform>((*physicsComponents)[j].entityID);
				if (transform != nullptr) {
					transform->angle = (*physicsComponents)[j].body->GetAngle();
					transform->x = (*physicsComponents)[j].body->GetPosition().x*stuff::SCALE;
                    transform->y = (*physicsComponents)[j].body->GetPosition().y*stuff::SCALE;
				}
			}
		}*/

	}
};

#endif // PUZZLE_SYSTEM_H_INCLUDED
