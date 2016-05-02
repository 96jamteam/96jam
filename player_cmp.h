#pragma once

#include"component.h"

class Player : public Component
{
public:
	int number;
	float speed;
	bool alive=1;
};
