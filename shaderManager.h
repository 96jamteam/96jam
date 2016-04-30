#pragma once
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <iostream>

class Spotlight
{
public:
    Spotlight(sf::Texture texture);
    ~Spotlight();
};


class PointLight
{
public:
    PointLight(sf::Texture texture);
    ~PointLight();
};
