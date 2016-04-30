/*#pragma once
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <iostream>
#include "views.h"
class Shader
{
private:
    sf::RenderWindow* window;
    Views* views;
public:
    void spotlight(sf::Texture texture);
    void pointLight(sf::Texture texture);
    void drawScreenUsingShader(sf::Shader shader);
    sf::Shader createDefaultShader();
    Shader(sf::RenderWindow* _window, Views* _views) : window(_window), views(_views){}
};
*/
