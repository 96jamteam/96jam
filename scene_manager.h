#ifndef SCENE_MANAGER_H_INCLUDED
#define SCENE_MANAGER_H_INCLUDED

#include <map>
#include "stuff.h"

class SceneManager {
public:
	enum State {
		sleep, active, destroy
	};

	static State getState(const std::string& name) {
		std::map< std::string, int>::iterator it = translation.find(name);
		if (it != translation.end())
			return data[it->second];
		else
			return State::destroy;
	}

	static State getState(const int& sceneID) {
		std::map< int, State>::iterator it = data.find(sceneID);
		if (data.find(sceneID) != data.end())
			return data[sceneID];
		else
			return State::destroy;
	}

	static void set(const State& reciver, const State& newState) {
		for (auto& dat : data) {
			if (dat.second == reciver)
				dat.second = newState;
		}
	}


	static void modState(const std::string& name, const State& state) {
		if (translation[name] == 0)
			return;
		modState(translation[name], state);
	}

	static void modState(const int& sceneID, const State& state) {

		if (state == State::destroy) {
			data.erase(sceneID);
			/*for (auto& trans : translation) {
                if (trans.second == sceneID) {
					translation.erase(trans.first);
				}
			}*/
		}
		else {
			data[sceneID] = state;
		}
	}

	static int addScene(const std::string& name, const State& state) {
		IDgen++;
		translation[name] = IDgen;
		data[IDgen] = state;
		return IDgen;
	}

	static int getID(const std::string& name) {
		return translation[name];
	}

	static void init() {
		IDgen = 0;
	}

private:
	static std::map<int, State> data;
	static std::map<std::string, int> translation;

	static int IDgen;
};

#endif // SCENE_MANAGER_H_INCLUDED
