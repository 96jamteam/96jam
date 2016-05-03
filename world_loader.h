#pragma once
#include "eventchannel.h"
#include "structures_for_broadcast.h"
#include "scene_manager.h"
#include "scene_cmp.h"
#include "pugi_wrapper.h"
#include "world_loader.h"

class Game;

class WorldLoader
{
	Game* game;
	int sceneID;
	int numOfPlayers;
	int lastOpenedLvl;
public:
	bool loadFromFile(const std::string&);
	std::string readSave(const std::string&,const std::string&);

	void handle(const LoadWorld& lw);
	void handle(const SaveLvl& lw);

    void loadSprite(XML&, const int &);
    void loadAnimatedSprite(XML&, const int &);
	void loadTransform(XML&, const int &);
    void loadBody(XML&, const int &);
    void loadPlayer(XML&, const int &);
    void loadWeapons(XML&, const int &);
    void loadBot(XML&, const int &);
    void loadContactName(XML&, const int &);
    void loadPuzzle(XML&, const int &);
    void loadRoslinka(XML&, const int &, bool);

	WorldLoader(Game* _game):game(_game){ mChannel.add<LoadWorld>(*this); mChannel.add<SaveLvl>(*this);}

	EventChannel mChannel;
};
