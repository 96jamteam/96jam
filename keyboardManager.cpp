#include "keyboardManager.h"
#include <map>
#include "pugi_wrapper.h"

void KeyboardManager::load (std::string s)
{
    XML plik;
    plik.load(s);
    for(auto& xml : plik.iter(""))
    {
        keys[xml->get<std::string>(":name")] = (xml->get<int>(":keycode"));
    }
}

bool KeyboardManager::isDown(std::string s)
{
    //P(keys[s]);
    return sf::Keyboard::isKeyPressed((sf::Keyboard::Key)keys[s]);
}
