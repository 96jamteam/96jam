#ifndef ENGINE_H
#define ENGINE_H

#include<list>
#include"system.h"
#include "views.h"
#include"eventchannel.h"
#include "views.h"

class Engine
{
private:
    Views* views;
    sf::RenderWindow* window;
    sf::Sprite gamesp, guisp;
    public:
        struct StopEvent {};
        Engine(){
            channel.add<StopEvent>(*this);
            running=0;
        }
    void setVariables(Views* _views, sf::RenderWindow* _window){
        views = _views;
        window = _window;
    }
        virtual ~Engine(){
        }
        void add(const std::shared_ptr<System>& sys){
            systems.push_back(sys);
        }
        bool isRunning(){
            return running;
        }

        void run(){
            running=true;
            while(running){
                for(auto sys: systems){
                    sys->update();
                }
                
                
                window->clear();
                gamesp = sf::Sprite(views->gameTexture.getTexture());
                window->draw(gamesp);
                guisp = sf::Sprite(views->guiTexture.getTexture());
                window->draw(guisp);
                window->display();
            }
        }

        void handle(const StopEvent&){
            stop();
        }

    private:
        void stop(){
            running=false;
        }
        bool running;
        std::list<std::shared_ptr<System>>systems;
        EventChannel channel;
};

#endif // ENGINE_H
