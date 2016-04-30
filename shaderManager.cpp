#include "shaderManager.h"
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <iostream>
Spotlight::Spotlight(sf::Texture texture, Game* game)
{
    this->game = game;
    if (sf::Shader::isAvailable())
    {
        sf::Shader spotShader;
        if (!spotShader.loadFromFile("shaders//spotlight.vertex", "shaders//spotlight.fragment"))
        {
            std::cout<<"Nie udalo sie zaladowac shaderow typu spotlight";
        }
        spotShader.setParameter("texture", texture);
    }
}

PointLight::PointLight(sf::Texture texture, Game* game)
{
    this->game = game;
    if(sf::Shader::isAvailable())
    {
        sf::Shader pointLightShader;
        if(!pointLightShader.loadFromFile("shaders//pointLight.vertex", "shaders//pointLight.fragment"))
        {
            std::cout<<"Nie uda³o sie zaladowac shaderow typu point light";
        }
        pointLightShader.setParameter("texture", texture);
    }
}

Spotlight::drawUsingShader(sf::Texture texture, sf::Shader shader)
{
    sf::Shader::bind(&shader);
    game->window.draw(game->views->gameTexture, &shader);
}

PointLight::drawUsingShader(sf::Texture texture, sf::Shader shader)
{
    sf::Shader::bind(&shader);
    game->window.draw(game->views->gameTexture, &shader);
}
