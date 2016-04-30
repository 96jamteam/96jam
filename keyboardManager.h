#pragma once
#include <map>
#include <SFML/Graphics.hpp>
class KeyboardManager
{
    public:
    std::map<std::string, int> keys;
        void load(std::string);
        bool isDown(std::string s);
};
