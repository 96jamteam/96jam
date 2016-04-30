#include <Box2D/Box2D.h>
#include <cstdio>
#include <iostream>
#include <SFML/Audio.hpp>
#include "engine.h"

Engine engine;

class Syss : public System{
    virtual void update(){
        std::cout<<"gowno\n";
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
