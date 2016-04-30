#pragma once

#include<iostream>
#include<SFML/Graphics.hpp>
#include "Box2D/Box2D.h"

#include "system.h"
#include "structures_for_broadcast.h"
#include "stuff.h"
#include "views.h"
#include "physics_cmp.h"

class DebugDrawSystem : public System, public b2Draw{

private:
	Views* views;
	b2World* world;
	float SCALE = stuff::SCALE;
	sf::RenderWindow* target;

public:
    DebugDrawSystem(b2World* w, sf::RenderWindow* t, Views* _views) : world(w),target(t),views(_views){
        world->SetDebugDraw(this);
        SetFlags(b2Draw::e_shapeBit /*| b2Draw::e_shapeBit*/);
	}

	virtual ~DebugDrawSystem() {

	}


	virtual void update() {
		target->setView(views->gameView);
		world->DrawDebugData();
	}

private:
	sf::Color GLColorToSFML(const b2Color &color, sf::Uint8 alpha = 255)
	{
		return sf::Color(static_cast<sf::Uint8>(color.r * 255), static_cast<sf::Uint8>(color.g * 255), static_cast<sf::Uint8>(color.b * 255), alpha);
	}

	/// Convert Box2D's vector to SFML vector [Default - scales the vector up by SCALE constants amount]
	sf::Vector2f B2VecToSFVec(const b2Vec2 &vector, bool scaleToPixels = true)
	{
		return sf::Vector2f(vector.x * (scaleToPixels ? SCALE : 1.f), vector.y * (scaleToPixels ? SCALE : 1.f));
	}

	/// Draw a closed polygon provided in CCW order.
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

	/// Draw a solid closed polygon provided in CCW order.
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

	/// Draw a circle.
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color);

	/// Draw a solid circle.
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color);

	/// Draw a line segment.
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

	/// Draw a transform. Choose your own length scale.
	void DrawTransform(const b2Transform& xf);
};