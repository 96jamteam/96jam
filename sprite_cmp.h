#ifndef SPRITE_CMP_H_INCLUDED
#define SPRITE_CMP_H_INCLUDED

#include"component.h"

class SpriteC : public Component
{
public:
	std::vector<sf::Sprite> sprites;
    std::vector<float> z;
};

#endif // SPRITE_CMP_H_INCLUDED
