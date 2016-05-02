//
//  bot_cmp.h
//  maybe
//
//  Created by Janek on 01/05/16.
//  Copyright © 2016 Janek. All rights reserved.
//

#ifndef bot_cmp_h
#define bot_cmp_h
#include"component.h"
#include <SFML/Graphics.hpp>

class Bot : public Component{
public:
    sf::Vector2f pos;
    bool isLooking;
    float speed;
    unsigned int heSees;
    sf::Vector2f target;

    float freeWalkingAngle;
};

#endif /* bot_cmp_h */
