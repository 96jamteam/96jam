#include "shaderManager.h"
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <iostream>
Spotlight::Spotlight()
{
    sf::RenderTexture renderTexture;
    if (sf::Shader::isAvailable())
    {
        sf::Shader spotShader;
        if (!spotShader.loadFromFile("shaders//spotlight.vertex", "shaders//spotlight.fragment"))
        {
            std::cout<<"Nie udalo sie zaladowac shaderow typu spotlight";
        }
        spotShader.setParameter("texture", sf::Shader::CurrentTexture);
    }
}

PointLight::PointLight()
{
    sf::RenderTexture renderTexture;
    if(sf::Shader::isAvailable())
    {
        sf::Shader pointLightShader;
        if(!pointLightShader.loadFromFile("shaders//pointLight.vertex", "shaders//pointLight.fragment"))
        {
            std::cout<<"Nie uda³o sie zaladowac shaderow typu point light";
        }
    }
}

