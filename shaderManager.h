#pragma once
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <iostream>
#include "game.h"
class Spotlight
{
private:
    Game* game;
public:
    Spotlight(sf::Texture texture, Game* game);
    ~Spotlight();

    drawUsingShader(sf::Texture texture, sf::Shader shader);
};


class PointLight
{
private:
    Game* game;
public:
    PointLight(sf::Texture texture, Game* game);
    ~PointLight();

     drawUsingShader(sf::Texture texture, sf::Shader shader);
};
