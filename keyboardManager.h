#pragma once
#include <map>
class KeyboardManager
{
    public:
        std::map<std::string, unsigned int> keys;
        void load(std::string);
};
