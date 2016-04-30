#pragma once

#include<map>
#include<unordered_map>
#include<iostream>
#include<deque>

#include"stuff.h"

namespace componentContainer {
	template <typename T>
	using container = std::vector<T>;
}

class Getter{
public:
	Getter() {}
	virtual ~Getter() { }
	virtual void* getStorage() = 0;
	virtual void deleteComponent(int entityID) = 0;
	virtual void activateComponent(bool active, int entityID) = 0;

	std::map<unsigned, unsigned> IDtable;

	inline std::map<unsigned, unsigned>& getIDtable() {
		return IDtable;
	}
};

template <class T>
class GetterFor : public Getter{
public:
	virtual ~GetterFor(){
		storage.clear();
		storage.shrink_to_fit();
	}

	virtual void* getStorage() {
		return &storage;
	}

	virtual void deleteComponent(int entityID) {
		if (IDtable.find(entityID) == IDtable.end())
			return;

		int index = IDtable[entityID];
		if (index != storage.size() - 1) {
			storage[index] = std::move(storage.back());
			IDtable[storage[index].entityID] = index;
		}

		storage.pop_back();
		IDtable.erase(entityID);
	}

	virtual void activateComponent(bool active, int entityID) {
		if (IDtable.find(entityID) == IDtable.end())
			return;

		storage[IDtable[entityID]].active = active;
	}

	componentContainer::container<T> storage;
};

class ComponentContainer {
public:
	ComponentContainer() : uniqueID(0){}
	~ComponentContainer() {
		clear();
	}

	inline unsigned getUniqueID() {
		return ++uniqueID;
	}

	template <typename T>
	inline componentContainer::container<T>* getComponentStorage() {
		if (components.count(&typeid(T)) != 0)
			return static_cast<componentContainer::container<T>*>(components[&typeid(T)]->getStorage());
		else
			return nullptr;
	}

	template <typename T>
	inline componentContainer::container<T>* addComponentStorage() {
		components[&typeid(T)] = new GetterFor<T>();
		return static_cast<componentContainer::container<T>*>(components[&typeid(T)]->getStorage());
	}

	template <typename T>
	inline int createComponent(int entityID) {

		componentContainer::container<T>* wsk = getComponentStorage<T>();

		if (wsk == nullptr)
			wsk = addComponentStorage<T>();

		wsk->emplace_back();
		wsk->back().entityID = entityID;
		components[&typeid(T)]->IDtable[entityID] = wsk->size() - 1;
		return  wsk->size() - 1;
	}

	template <typename T>
	inline T* getComponent(int entityID) {

		componentContainer::container<T>* wsk = getComponentStorage<T>();

		if (wsk == nullptr)
			return nullptr;

		if( components[&typeid(T)]->IDtable.count(entityID) != 0)
			return &((*wsk)[components[&typeid(T)]->IDtable[entityID]]);
		else return nullptr;
	}

	template <typename T>
	inline void deleteComponent(int entityID) {
		components[&typeid(T)]->deleteComponent(entityID);
	}

	inline void deleteComponents(int entityID) {
		typedef std::unordered_map<const std::type_info*, Getter *>::iterator it_type;
		for (it_type iterator = components.begin(); iterator != components.end(); iterator++) {
			iterator->second->deleteComponent(entityID);
		}
	}

	inline void activateComponents(bool active, int entityID) {
		typedef std::unordered_map<const std::type_info*, Getter *>::iterator it_type;
		for (it_type iterator = components.begin(); iterator != components.end(); iterator++) {
			iterator->second->activateComponent(active, entityID);
		}
	}

private:
	std::unordered_map<const std::type_info*, Getter *> components;

	void clear() {
		typedef std::unordered_map<const std::type_info*, Getter *>::iterator it_type;
		for (it_type iterator = components.begin(); iterator != components.end(); iterator++) {
			delete iterator->second;
		}
	}

	unsigned uniqueID;
};
