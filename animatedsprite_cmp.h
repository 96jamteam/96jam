#pragma once

#include"component.h"
#include "AnimatedSprite.hpp"

class AnimatedSpriteC : public Component
{
public:
	std::vector<AnimatedSprite> sprites;
    std::vector<float> z;
};