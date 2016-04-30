//
//  test_system.hpp
//  maybe
//
//  Created by Janek on 30/04/16.
//  Copyright © 2016 Janek. All rights reserved.
//

#ifndef test_system_hpp
#define test_system_hpp

#include <stdio.h>
#include"system.h"
#include"engine.h"

#include"game.h"

class TestSystem : public System{
    
    sf::RenderWindow* win;
    Game* game;
    sf::Event event;
    sf::Sprite sp;
    
public:
    TestSystem(sf::RenderWindow& _win, Game* _game):System(){
        win=&_win;
        game=_game;
        sp = sf::Sprite();
        sp.setTexture(*game->Textures.Get("gawron"));
        sp.setScale(0.3f, 0.3f);
        
    }
    
    void update(){
        win->draw(sp);
    }
};
#endif /* test_system_hpp */
