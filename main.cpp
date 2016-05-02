#include "game.h"
Game game;
int main(int argc, char** argv)
{
    srand(time(NULL));

    game.run();
    return EXIT_SUCCESS;
}
