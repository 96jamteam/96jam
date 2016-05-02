//
//  AnimationManager.cpp
//  heist
//
//  Created by Janek on 25/12/15.
//  Copyright (c) 2015 Janek. All rights reserved.
//

#include "AnimationManager.h"
#include <iostream>
#include <string.h>

Animation* AnimationManager::getAnimation(const std::string& name){
    return &animations.at(name);
}

bool AnimationManager::addFromPath(const std::string& path, Manager<sf::Texture, stuff::MAX_TEXTURES> &manager){

    XML doc;
    doc.load(path);


    for (auto & anim_xml: doc.iter("animations")){

        Animation animation;
        animation.setSpriteSheet(*manager.Get(anim_xml->get<std::string>(":texture")));
        P(anim_xml->get<std::string>(":texture"));

        if(strcmp((anim_xml->get<std::string>(":rows")).c_str(), "") == 0){ // eee #głupieC++
            for (auto & frame: anim_xml->iter("")){
                animation.addFrame(sf::IntRect(frame->get<int>(":x"), frame->get<int>(":y"), frame->get<int>(":w"), frame->get<int>(":h")));
            }
        }else{
            for(int y = 0; y < anim_xml->get<int>(":rows"); y++){
                for(int x = 0; x < anim_xml->get<int>(":cols"); x++){
                    animation.addFrame(sf::IntRect(anim_xml->get<int>(":width")*(x), anim_xml->get<int>(":height")*(y), anim_xml->get<int>(":width"), anim_xml->get<int>(":height")));
                    P("WWWW " << anim_xml->get<int>(":offsetX"));
                    P("x: " << anim_xml->get<int>(":width")*(x));
                      P("y: " << anim_xml->get<int>(":height")*(y));
                      P("h: " <<anim_xml->get<int>(":height"));
                }
            }
        }
        animations.insert(std::make_pair(anim_xml->get<std::string>(":name"), animation));
    }
    return true;
}
