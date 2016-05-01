#ifndef WINDOW_SYSTEM_H_INCLUDED
#define WINDOW_SYSTEM_H_INCLUDED

#include"system.h"
#include"engine.h"
#include"views.h"
#include"structures_for_broadcast.h"
#include"scene_manager.h"
#include "stuff.h"

class WindowSystem : public System{
    sf::RenderWindow* win;
    Views* views;
    sf::Event event;
public:
    WindowSystem(sf::RenderWindow& _win,Views& _views):System(){
        win=&_win;
        views=&_views;
    }

    void update(){
        win->display();
        win->clear();
        while (win->pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                win->close();
                mChannel.broadcast(Engine::StopEvent());
            }
            else if (event.type == sf::Event::MouseMoved){
				sf::Vector2i pixelPos = sf::Mouse::getPosition(*win);
				sf::Vector2f worldPos = win->mapPixelToCoords(pixelPos, views->guiView);
				mChannel.broadcast( MousePos(float(worldPos.x), float(worldPos.y)) );
			}
			else if (event.type == sf::Event::KeyPressed){
				mChannel.broadcast(KeyEvent(event.key.code,true));
			}
			else if (event.type == sf::Event::KeyReleased) {
				mChannel.broadcast(KeyEvent(event.key.code, false));
			}
			else if (event.type == sf::Event::MouseButtonPressed) {
				mChannel.broadcast(MouseButtonEvent(event.mouseButton.button, true));
				//SceneManager::modState("game",SceneManager::State::sleep);
				//mChannel.broadcast(SceneUpdate());
			}
			else if (event.type == sf::Event::MouseButtonReleased) {
				mChannel.broadcast(MouseButtonEvent(event.mouseButton.button, false));
				//SceneManager::modState("game",SceneManager::State::active);
				//mChannel.broadcast(SceneUpdate());
			}
			else if (event.type == sf::Event::Resized) {
				views->gameView = views->getLetterboxView(views->gameView, event.size.width, event.size.height);
				views->guiView = views->getLetterboxView(views->guiView, event.size.width, event.size.height);
			}
        }
    }
};

#endif // WINDOW_SYSTEM_H_INCLUDED
