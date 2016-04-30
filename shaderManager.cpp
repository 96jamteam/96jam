#include "shaderManager.h"
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <iostream>
void Shader::spotlight(sf::Texture texture)
{
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

void Shader::pointLight(sf::Texture texture)
{
    if(sf::Shader::isAvailable())
    {
        sf::Shader pointLightShader;
        if(!pointLightShader.loadFromFile("shaders//pointLight.vertex", "shaders//pointLight.fragment"))
        {
            std::cout<<"Nie udalo sie zaladowac shaderow typu point light";
        }
        pointLightShader.setParameter("texture", texture);
    }
}

void Shader::drawScreenUsingShader(sf::Shader shader)
{
    sf::Shader::bind(&shader);
    sf::Sprite sp;
    sp.setTexture(views->gameTexture.getTexture());
    window->draw(sp, &shader);
}
 sf::Shader Shader::createDefaultShader()
 {
     if(sf::Shader::isAvailable())
    {
        sf::Shader shader;
        if (!shader.loadFromFile("shaders//default.vertex", "shaders//default.fragment"))
            {
                std::cout<<"Nie udalo sie zaladowac shaderow typu default";
            }
    }
 }
