#include <Box2D/Box2D.h>
#include <cstdio>
#include <iostream>
#include <SFML/Audio.hpp>
#include "engine.h"
#include "game.h"

Game game;



int main(int argc, char** argv)
{
    game.run();
    return EXIT_SUCCESS;
}
