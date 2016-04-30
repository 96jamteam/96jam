#include "game.h"

Game::Game()
{
    std::shared_ptr<Syss>s( new Syss() );
    engine.add(s);
    sf::RenderWindow window(sf::VideoMode(800, 600,32), "7th96hJ",sf::Style::Default);
    sf::ContextSettings settings = window.getSettings();
    std::cout << "OpenGl"<<settings.majorVersion << "." << settings.minorVersion << std::endl;
}
