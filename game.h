#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include "engine.h"
#include "pugi_wrapper.h"

class Syss : public System{
    
    std::string s;
    
public:
    Syss(){
        XML xml;
        xml.load("word.xml");
        this->s = xml.get<std::string>("word:str");
    }
    
    virtual void update(){
        std::cout<<this->s<<std::endl;
        EventChannel chan;
        chan.broadcast(Engine::StopEvent());
    }
};

class Game
{
public:
    Game();
    void run()
    {
        engine.run();
    }
    sf::RenderWindow window;
    Engine engine;
private:
};

#endif // GAME_H
