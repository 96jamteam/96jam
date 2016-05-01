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
    sf::Vector2f vel;
    float speed;
};

#endif /* bot_cmp_h */
