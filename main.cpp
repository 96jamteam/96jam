#include <Box2D/Box2D.h>
#include <cstdio>
#include <iostream>
#include <SFML/Audio.hpp>
#include "engine.h"
#include "pugi_wrapper.h"

Engine engine;

class Syss : public System{
    
    std::string s;
    
    public:
    Syss(){
        XML xml;
        xml.load("word.xml");
        std::cout << xml.get<std::string>("words.word:str");
        this->s = xml.get<std::string>("word:str");
    }
    
    virtual void update(){
        std::cout<<this->s<<std::endl;
        EventChannel chan;
        chan.broadcast(Engine::StopEvent());
    }
};

int main(int argc, char** argv)
{
    std::shared_ptr<Syss>s( new Syss() );
    engine.add(s);
    engine.run();
    return EXIT_SUCCESS;
}
