#pragma once
#include"component.h"

#include "weapons.h"

class Weapon;

class WeaponC : public Component
{
public:
	std::vector<Weapon> weapons;
	int active_weapon;
};

///WEAPON
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include "AnimatedSprite.hpp"

struct AnimOrSprite {
	bool graphicsType;
	AnimatedSprite animation;
	sf::Sprite sprite;
};

struct Shape {
	bool shapeType;
	std::shared_ptr<b2PolygonShape> polygon;
	std::shared_ptr<b2CircleShape> circle;
};

class Weapon {
public:
	float maxCooldown;
	float cooldown = 0;
	float speed;
	int spawnBullets; //ilosc spawnujacych sie pociskow za jednym strzalem
	float spread; //rozrzut, podany jako kat miedzy idealnym strzalem, a max odchyleniem
	int magazine; //ilosc kul w magazynku

	float bulletDmg; //obrazenia zadawane przez kule

	b2Vec2 spawnPoint;

	std::string animTexName; //nazwa animacji lub textury
	AnimOrSprite bulletGraphics;

	b2BodyDef bulletBodyDef;
	b2FixtureDef bulletFixDef;
	Shape bulletShape;
};
