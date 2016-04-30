#pragma once

#include"component.h"

#include <Box2D/Box2D.h>

class Physics : public Component
{
public:
	std::shared_ptr<b2Body> body;
	//b2Body *body = nullptr;
};
