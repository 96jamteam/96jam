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

    bool allDone(){
        int d=0,m=0;
        for (int j = 0; j < puzzles->size(); j++) {
			if (!(*puzzles)[j].active)
                continue;
            if ((*puzzles)[j].done || (*puzzles)[j].name=="rescue"){
                    d++;
            }
            m++;
		}
		if(d>=m)return true;
		else return false;

    }

	virtual void update() {
	    int done=0,max=0;
	    for (int j = 0; j < puzzles->size(); j++) {
			if (!(*puzzles)[j].active)
                continue;
            if ((*puzzles)[j].done){
                if((*puzzles)[j].name=="rescue"){
                    if(allDone())
                        done++;
                    else
                        (*puzzles)[j].done=0;
                }
                else done++;
            }
            max++;
		}
        if(done==max && max){
            //SceneManager::modState("game", SceneManager::State::destroy);
            SceneManager::modState("win_scene", SceneManager::State::active);

            mChannel.broadcast(SaveLvl());
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
