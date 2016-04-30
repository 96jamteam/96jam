#include "keyboardManager.h"
#include <map>
#include "pugi_wrapper.h"

void KeyboardManager::load (std::string s)
{
    XML plik;
    plik.load(s);
    for(auto& xml : plik.iter(""))
    {
        keys[xml->get<std::string>(":name")] = xml->get<int>(":keycode");
    }
}
