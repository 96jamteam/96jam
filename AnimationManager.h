//
//  AnimationManager.h
//  heist
//
//  Created by Janek on 25/12/15.
//  Copyright (c) 2015 Janek. All rights reserved.
//

#ifndef __heist__AnimationManager__
#define __heist__AnimationManager__

#include <stdio.h>
#include <map>
#include "Animation.hpp"
#include "ResourceManagerWrapper.hpp"
#include <SFML/Graphics.hpp>
#include "stuff.h"
#include "pugi_wrapper.h"

class AnimationManager{
public:
    Animation* getAnimation(const std::string&);
    bool addFromPath(const std::string&, Manager<sf::Texture, stuff::MAX_TEXTURES>&);

private:
    std::map<std::string, Animation> animations;
};

#endif /* defined(__heist__AnimationManager__) */
