#ifndef WINDOW_SYSTEM_H_INCLUDED
#define WINDOW_SYSTEM_H_INCLUDED
#include"system.h"
#include"engine.h"

class WindowSystem : public System{

    sf::RenderWindow* win;
    sf::Event event;
public:
    WindowSystem(sf::RenderWindow& _win):System(){
        win=&_win;
    }

    void update(){
        win->display();
        win->clear();
        while (win->pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                win->close();
                EventChannel chan;
                chan.broadcast(Engine::StopEvent());
            }
        }
    }
};

#endif // WINDOW_SYSTEM_H_INCLUDED
