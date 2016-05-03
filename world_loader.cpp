#include "world_loader.h"
#include "game.h"
#include "player_cmp.h"
#include "weapon_cmp.h"
#include "contact_name_cmp.h"
#include "sprite_cmp.h"
#include "contact_name_cmp.h"
#include "callback_cmp.h"
#include "animatedsprite_cmp.h"
#include "bot_cmp.h"
#include "puzzle_cmp.h"
#include<sstream>
#include <fstream>


bool WorldLoader::loadFromFile(const std::string& path)
{
	numOfPlayers = 0;
    XML xml;
    xml.load(path);

    for(auto& entity_xml : xml.iter("entities")){

		int entity = game->container.getUniqueID();

		game->container.createComponent<Scene>(entity);
		game->container.getComponent<Scene>(entity)->sceneID = sceneID;
        game->container.createComponent<ContactName>(entity);
        game->container.getComponent<ContactName>(entity)->name = entity_xml->get<std::string>(":name");

        for(auto& cmp_xml : entity_xml->iter("")){

            if(cmp_xml->getName() == "sprites"){
                loadSprite(*cmp_xml, entity);
            }
            if(cmp_xml->getName() == "animations"){
                loadAnimatedSprite(*cmp_xml, entity);
            }
            if(cmp_xml->getName() == "transform"){
                loadTransform(*cmp_xml, entity);
            }
			if (cmp_xml->getName() == "body") {
				loadBody(*cmp_xml, entity);
			}
            if (cmp_xml->getName() == "player") {
                loadPlayer(*cmp_xml, entity);
            }
			if (cmp_xml->getName() == "weapons") {
				loadWeapons(*cmp_xml, entity);
			}
            if (cmp_xml->getName() == "bot") {
                loadBot(*cmp_xml, entity);
            }
            if (cmp_xml->getName() == "contact_name") {
                loadContactName(*cmp_xml, entity);
            }
            if (cmp_xml->getName() == "puzzle") {
                loadPuzzle(*cmp_xml, entity);
            }
            if (cmp_xml->getName() == "roslinka"){
                loadRoslinka(*cmp_xml, entity, true);
            }
            if (cmp_xml->getName() == "roslinka2"){
                loadRoslinka(*cmp_xml, entity, false);
            }
        }
    }
	//mChannel.broadcast(StartMusic());
	mChannel.broadcast(SceneUpdate());
	return false;
}

void WorldLoader::handle(const LoadWorld & LWE)
{
	sceneID = LWE.id;
    if(LWE.type=="normal")
        loadFromFile(readSave("levels//save.xml",LWE.type));
    else
        loadFromFile(std::string("levels//")+LWE.type+std::string(".xml"));
}

void WorldLoader::handle(const SaveLvl& lw){
    XML xml;
    xml.load("levels//save.xml");
    int level = xml.get<int>("save:level");
    if(level<=lastOpenedLvl){
        std::ofstream outfile;
        outfile.open("levels//save.xml");
        outfile << std::string(" <save level=\"") <<lastOpenedLvl+1<<std::string("\"></save> ") << std::endl;
        outfile.close();
    }
}

std::string WorldLoader::readSave(const std::string& file, const std::string& type){
    XML xml;
    xml.load(file);
    int level = xml.get<int>("save:level");

    if(level>3){
        //SceneManager::modState("game",SceneManager::destroy);
        SceneManager::set(SceneManager::State::active, SceneManager::State::sleep);
        SceneManager::modState("credits",SceneManager::active);
        mChannel.broadcast(SceneUpdate());
    }


    lastOpenedLvl=level;
    std::stringstream ss;
    ss << level;
    return std::string("levels//")+ss.str()+std::string(".xml");
}

void WorldLoader::loadContactName(XML& xml, const int & entity){
    game->container.createComponent<ContactName>(entity);
    game->container.getComponent<ContactName>(entity)->name = xml.get<std::string>(":name");
}

void WorldLoader::loadPuzzle(XML&xml, const int &entity){
    game->container.createComponent<Puzzle>(entity);
    game->container.getComponent<Puzzle>(entity)->name = xml.get<std::string>(":name");
    game->container.getComponent<Puzzle>(entity)->done = 0;

    game->container.createComponent<CallbackCmp>(entity);
    game->container.getComponent<CallbackCmp>(entity)->callbacks[xml.get<std::string>(":activator")]=([this,entity](){
           game->container.getComponent<Puzzle>(entity)->done=1;
            });

}

void WorldLoader::loadSprite(XML& xml, const int & entity)
{
	game->container.createComponent<SpriteC>(entity);
    for(auto& tmp_xml : xml.iter("")){
        sf::Sprite sprite(*game->Textures.Get(tmp_xml->get<std::string>(":texture")));

        if (tmp_xml->get<std::string>(":scale") != ""){
            sprite.setScale(tmp_xml->get<float>(":scale"), tmp_xml->get<float>(":scale"));
        }
        if (tmp_xml->get<std::string>(":origin_x") != ""){
            sprite.setOrigin(tmp_xml->get<float>(":origin_x"), tmp_xml->get<float>(":origin_y"));
        }
		game->container.getComponent<SpriteC>(entity)->sprites.push_back(sprite);
		game->container.getComponent<SpriteC>(entity)->z.push_back(tmp_xml->get<float>(":z"));

    }
    mChannel.broadcast(SpriteAdded());
}

void WorldLoader::loadAnimatedSprite(XML& xml, const int & entity)
{
	game->container.createComponent<AnimatedSpriteC>(entity);

    for(auto& tmp_xml : xml.iter("")){
        AnimatedSprite sprite;
        sprite.setAnimation(*game->Animations.getAnimation(tmp_xml->get<std::string>(":animation")));

        sprite.setFrameTime(sf::seconds(1/12.f));
        sprite.play();

        if (tmp_xml->get<std::string>(":scale") != ""){
            sprite.setScale(tmp_xml->get<float>(":scale"), tmp_xml->get<float>(":scale"));
        }
        if (tmp_xml->get<std::string>(":origin_x") != ""){
            sprite.setOrigin(tmp_xml->get<float>(":origin_x"), tmp_xml->get<float>(":origin_y"));
        }

		game->container.getComponent<AnimatedSpriteC>(entity)->sprites.push_back(sprite);
		game->container.getComponent<AnimatedSpriteC>(entity)->z.push_back(tmp_xml->get<float>(":z"));
    }
    mChannel.broadcast(SpriteAdded());
}

void WorldLoader::loadTransform(XML & xml, const int & entity)
{
	game->container.createComponent<Transform>(entity);
	game->container.getComponent<Transform>(entity)->x = xml.get<float>(":x");
	game->container.getComponent<Transform>(entity)->y = xml.get<float>(":y");
	game->container.getComponent<Transform>(entity)->angle =  xml.get<float>(":angle");
}

void WorldLoader::loadPlayer(XML& xml, const int & entity)
{
	game->container.createComponent<ContactName>(entity);
    game->container.getComponent<ContactName>(entity)->name = "player";

    game->container.createComponent<CallbackCmp>(entity);
    game->container.getComponent<CallbackCmp>(entity)->callbacks["pudlo_begin"]=([](){
           // EventChannel chan;
            //chan.broadcast(Engine::StopEvent());
            });
    game->container.getComponent<CallbackCmp>(entity)->callbacks["bot_begin"]=([this,entity](){
        // EventChannel chan;
        mChannel.broadcast(KillPlayer(entity));
        SceneManager::modState("game_over",SceneManager::active);
        mChannel.broadcast(SceneUpdate());
    });

	game->container.createComponent<Player>(entity);
	game->container.getComponent<Player>(entity)->speed = xml.get<float>(":speed");
	game->container.getComponent<Player>(entity)->number = numOfPlayers++;
}

void WorldLoader::loadBot(XML& xml, const int & entity)
{
    game->container.createComponent<ContactName>(entity);
    game->container.getComponent<ContactName>(entity)->name = "bot";

    game->container.createComponent<Bot>(entity);
    game->container.getComponent<Bot>(entity)->speed = xml.get<float>(":speed");
    game->container.getComponent<Bot>(entity)->heSees = 2147483647;
    game->container.getComponent<Bot>(entity)->target.x = -69.6969696969f;
    game->container.getComponent<Bot>(entity)->freeWalkingAngle = float(rand()%10000)/10000.f*2*stuff::PI;

    game->container.createComponent<CallbackCmp>(entity);
    game->container.getComponent<CallbackCmp>(entity)->callbacks["bullet_begin"]=([this, entity](){
         EventChannel chan;
        chan.broadcast(DeleteThisShit(entity));
    });
}

void WorldLoader::loadRoslinka(XML& xml, const int& entity, bool isTutorial){
    if(isTutorial){
        game->container.createComponent<CallbackCmp>(entity);
        game->container.createComponent<Puzzle>(entity);
        game->container.getComponent<Puzzle>(entity)->name = xml.get<std::string>(":name");
        game->container.getComponent<Puzzle>(entity)->done = 0;
        game->container.getComponent<CallbackCmp>(entity)->callbacks["bullet_begin"]=([this, entity](){
            game->container.getComponent<SpriteC>(entity)->sprites[0].setTexture(*(game->Textures.Get("roslinka1-1-2")));
            game->container.getComponent<Puzzle>(entity)->done=1;
        });
    }else{
        game->container.createComponent<CallbackCmp>(entity);
        game->container.createComponent<Puzzle>(entity);
        game->container.getComponent<Puzzle>(entity)->name = xml.get<std::string>(":name");
        game->container.getComponent<Puzzle>(entity)->done = 0;
        game->container.getComponent<CallbackCmp>(entity)->callbacks["bullet_begin"]=([this, entity](){
            game->container.getComponent<SpriteC>(entity)->sprites[0].setTexture(*(game->Textures.Get("roslinka2")));
            game->container.getComponent<Puzzle>(entity)->done=1;
        });
    }
}

void WorldLoader::loadWeapons(XML& xml, const int & entity) {
	game->container.createComponent<WeaponC>(entity);

	for (auto& tmp_xml : xml.iter("")) {
		Weapon w;
		game->container.getComponent<WeaponC>(entity)->weapons.push_back(w);
		Weapon* weapon = &game->container.getComponent<WeaponC>(entity)->weapons[game->container.getComponent<WeaponC>(entity)->weapons.size() - 1];
		weapon->maxCooldown = tmp_xml->get<float>(":cooldown");
		weapon->speed = tmp_xml->get<float>(":speed");
		weapon->spawnBullets = tmp_xml->get<int>(":spawnBullets");
		weapon->magazine = tmp_xml->get<int>(":magazine");
		weapon->spread = tmp_xml->get<float>(":spread");
		weapon->bulletDmg = tmp_xml->get<float>(":bulletDmg");
		weapon->animTexName = tmp_xml->get<std::string>(":bulletAnimTexName");
		weapon->bulletDmg = tmp_xml->get<float>(":bulletDmg");
		weapon->spawnPoint.x = tmp_xml->get<float>(":spawnPoint_x");
		weapon->spawnPoint.y = tmp_xml->get<float>(":spawnPoint_y");

		if (tmp_xml->get<std::string>(":bulletAnimation") != "") {
			weapon->bulletGraphics.graphicsType = 0;
			weapon->bulletGraphics.animation.setAnimation(*game->Animations.getAnimation(weapon->animTexName));
			if (tmp_xml->get<std::string>(":origin_x") != "") {
				weapon->bulletGraphics.animation.setOrigin(tmp_xml->get<float>(":origin_x"), tmp_xml->get<float>(":origin_y"));
			}
		}
		else {
			weapon->bulletGraphics.graphicsType = 1;
			weapon->bulletGraphics.sprite.setTexture(*game->Textures.Get(weapon->animTexName));
			if (tmp_xml->get<std::string>(":origin_x") != "") {
				weapon->bulletGraphics.sprite.setOrigin(tmp_xml->get<float>(":origin_x"), tmp_xml->get<float>(":origin_y"));
			}
		}

		std::string body_type = tmp_xml->get<std::string>("body:type");
		if (body_type.find("dynamic") != std::string::npos)
			weapon->bulletBodyDef.type = b2_dynamicBody;
		else if (body_type.find("static") != std::string::npos)
			weapon->bulletBodyDef.type = b2_staticBody;
		else
			weapon->bulletBodyDef.type = b2_kinematicBody;

		weapon->bulletBodyDef.angularDamping = tmp_xml->get<float>("body:angularDamping");
		weapon->bulletBodyDef.linearDamping = tmp_xml->get<float>("body:linearDamping");
		weapon->bulletBodyDef.bullet = tmp_xml->get<bool>("body:bullet");
		if (tmp_xml->get<std::string>("body:fixedRotation") != "")
			weapon->bulletBodyDef.fixedRotation = tmp_xml->get<bool>("body:fixedRotation");

		weapon->bulletFixDef.density = tmp_xml->get<float>("body.fixture:density");
		weapon->bulletFixDef.friction = tmp_xml->get<float>("body.fixture:friction");
		weapon->bulletFixDef.restitution = tmp_xml->get<float>("body.fixture:restitution");
		weapon->bulletFixDef.isSensor = tmp_xml->get<bool>("body.fixture:isSensor");
		if (tmp_xml->get<bool>("filter") == true) {
			weapon->bulletFixDef.filter.categoryBits = tmp_xml->get<short>("body.fixture:categoryBits");
			weapon->bulletFixDef.filter.groupIndex = tmp_xml->get<short>("body.fixture:groupIndex");
			weapon->bulletFixDef.filter.maskBits = tmp_xml->get<short>("body.fixture:maskBits");
		}

		std::string body_type2 = tmp_xml->get<std::string>("body.fixture:shape");
		if (body_type2.find("box") != std::string::npos) {
			weapon->bulletShape.polygon = std::make_shared<b2PolygonShape>();
			weapon->bulletShape.polygon->SetAsBox(tmp_xml->get<float>("body.fixture:width") / stuff::SCALE / 2.f, tmp_xml->get<float>("body.fixture:height") / stuff::SCALE / 2.f);
			weapon->bulletFixDef.shape = weapon->bulletShape.polygon.get();
		}
		else if (body_type2.find("polygon") != std::string::npos) {
			weapon->bulletShape.polygon = std::make_shared<b2PolygonShape>();
			std::vector<b2Vec2> vec;
			int count = 0;
			//oh fuck
			for (auto& polygon_v_xml : tmp_xml->iter(""))
			for (auto& polygon_v_xml2 : polygon_v_xml->iter(""))
			for (auto& polygon_v_xml3 : polygon_v_xml2->iter("")) {
				vec.push_back(b2Vec2(polygon_v_xml3->get<float>(":x") / stuff::SCALE, polygon_v_xml3->get<float>(":y") / stuff::SCALE));
				count++;
			}
			weapon->bulletShape.polygon->Set(&vec[0], count);
			weapon->bulletFixDef.shape = weapon->bulletShape.polygon.get();
		}
		else if (body_type2.find("circle") != std::string::npos) {
			weapon->bulletShape.circle = std::make_shared<b2CircleShape>();
			weapon->bulletShape.circle->m_p.Set(tmp_xml->get<float>("body.fixture:relative_pos_x") / stuff::SCALE, tmp_xml->get<float>("body.fixture:relative_pos_y") / stuff::SCALE);
			weapon->bulletShape.circle->m_radius = tmp_xml->get<float>("body.fixture:radius") / stuff::SCALE;
			weapon->bulletFixDef.shape = weapon->bulletShape.circle.get();
		}


		game->container.getComponent<WeaponC>(entity)->active_weapon = 0;
	}

}

void WorldLoader::loadBody(XML& xml, const int & entity)
{
	game->container.createComponent<Physics>(entity);
	b2BodyDef def;


	//typ ciala
	std::string body_type = xml.get<std::string>(":type");
	if (body_type.find("dynamic") != std::string::npos)
		def.type = b2_dynamicBody;
	else if (body_type.find("static") != std::string::npos)
		def.type = b2_staticBody;
	else
		def.type = b2_kinematicBody;

	def.angularDamping = xml.get<float>(":angularDamping");
	def.linearDamping = xml.get<float>(":linearDamping");
	def.bullet = xml.get<bool>(":bullet");
    if (xml.get<std::string>(":fixedRotation") != "")
        def.fixedRotation = xml.get<bool>(":fixedRotation");

	//przypisanie poz z transform, transform musi byc deklarowany wczesniej niz body
	Transform* transform = game->container.getComponent<Transform>(entity);
	if (transform != nullptr) {
		def.position = b2Vec2(transform->x / stuff::SCALE, transform->y / stuff::SCALE);
		def.angle = stuff::degtorad(transform->angle);
	}

	game->container.getComponent<Physics>(entity)->body = game->world->CreateBody(&def);

    //entity id do user data, bo contact_listener
    game->container.getComponent<Physics>(entity)->body->SetUserData( (void*) game->container.getComponent<Physics>(entity)->entityID);

	//ladowanie fixtures i shapes
	for (auto& tmp_xml : xml.iter("")) {
		b2FixtureDef fix_def;
		fix_def.density = tmp_xml->get<float>(":density");
		fix_def.friction = tmp_xml->get<float>(":friction");
		fix_def.restitution = tmp_xml->get<float>(":restitution");
		fix_def.isSensor = tmp_xml->get<bool>(":isSensor");
		if (tmp_xml->get<bool>("filter") == true) {
			fix_def.filter.categoryBits = tmp_xml->get<short>(":categoryBits");
			fix_def.filter.groupIndex = tmp_xml->get<short>(":groupIndex");
			fix_def.filter.maskBits = tmp_xml->get<short>(":maskBits");
		}

		std::string body_type = tmp_xml->get<std::string>(":shape");
		if (body_type.find("box") != std::string::npos) {
			b2PolygonShape box;
			box.SetAsBox(tmp_xml->get<float>(":width") / stuff::SCALE /2.f, tmp_xml->get<float>(":height") / stuff::SCALE / 2.f);

			fix_def.shape = &box;
			game->container.getComponent<Physics>(entity)->body->CreateFixture(&fix_def);
		}
		else if (body_type.find("polygon") != std::string::npos) {
			b2PolygonShape polygon;
			std::vector<b2Vec2> vec;
			int count = 0;
			for (auto& polygon_v_xml : tmp_xml->iter("")) {
				vec.push_back(b2Vec2(polygon_v_xml->get<float>(":x") / stuff::SCALE, polygon_v_xml->get<float>(":y") / stuff::SCALE));
				count++;
			}
			polygon.Set(&vec[0], count);

			fix_def.shape = &polygon;
			game->container.getComponent<Physics>(entity)->body->CreateFixture(&fix_def);
		}
		else if (body_type.find("circle") != std::string::npos) {
			b2CircleShape circle;
			circle.m_p.Set(tmp_xml->get<float>(":relative_pos_x") / stuff::SCALE, tmp_xml->get<float>(":relative_pos_y") / stuff::SCALE);
			circle.m_radius = tmp_xml->get<float>(":radius") / stuff::SCALE;
			fix_def.shape = &circle;
			game->container.getComponent<Physics>(entity)->body->CreateFixture(&fix_def);
		}

	}
}
