#ifndef SCENE_SYSTEM_H_INCLUDED
#define SCENE_SYSTEM_H_INCLUDED

#include "system.h"
#include "structures_for_broadcast.h"
#include "stuff.h"
#include "scene_cmp.h"
#include "scene_manager.h"

class SceneSystem : public System {

private:
	ComponentContainer* cc;
	componentContainer::container<Scene>* components;
public:
	SceneSystem(ComponentContainer& _cc) : cc(&_cc) {
		mChannel.add<SceneUpdate>(*this);
		components = cc->getComponentStorage<Scene>();
		if (components == nullptr) {
			components = cc->addComponentStorage<Scene>();
		}
	}


	void handle(const SceneUpdate& su) {
		for (int j = 0; j < components->size(); j++) {

			if (SceneManager::getState((*components)[j].sceneID) == SceneManager::State::destroy) {
				cc->deleteComponents((*components)[j].entityID);
				j--;
			}
			else if (SceneManager::getState((*components)[j].sceneID) == SceneManager::State::sleep) {
				cc->activateComponents(false, (*components)[j].entityID);
			}
			else if (SceneManager::getState((*components)[j].sceneID) == SceneManager::State::active) {
				cc->activateComponents(true, (*components)[j].entityID);
			}

		}
		mChannel.broadcast(SpriteAdded());

	}

	virtual void update() {
		bool change = false;
		for (int j = 0; j < components->size(); j++) {
			if (SceneManager::getState((*components)[j].sceneID) == SceneManager::State::destroy) {
				cc->deleteComponents((*components)[j].entityID);
				j--;
				change = true;
			}
		}
		if (change)
			mChannel.broadcast(SpriteAdded());
	}
};

#endif // SCENE_SYSTEM_H_INCLUDED
