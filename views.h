#pragma once

#include <SFML/Graphics.hpp>
#include <deque>
#include "stuff.h"

class Views {
public:
	int VIEW_WIDTH;
	int VIEW_HEIGHT;

	int WINDOW_WIDTH;
	int WINDOW_HEIGHT;

	sf::View gameView;
	sf::View guiView;
    sf::RenderTexture gameTexture;
    sf::RenderTexture guiTexture;

	inline void addTarget(const sf::Vector2f& vec) {
		targets.push_back(vec);
	}

	inline void clearTargets() {
		targets.clear();
	}

	inline void setCenter() {
		if (targets.size() < 1)
			return;
		sf::Vector2f min(targets[0].x, targets[0].y);
		sf::Vector2f max(targets[0].x, targets[0].y);
        
		for (int f = 1; f < targets.size(); f++) {
			if (targets[f].x < min.x)
				min.x = targets[f].x;
			if (targets[f].y < min.y)
				min.y = targets[f].y;
			if (targets[f].x > max.x)
				max.x = targets[f].x;
			if (targets[f].y > max.y)
				max.y = targets[f].y;
		}
		gameView.setCenter(sf::Vector2f(min.x + (max.x - min.x) / 2.f, min.y + (max.y - min.y) / 2.f));
	}
    
    void declareTextures(int w, int h){
        gameTexture.create(w, h);
        guiTexture.create(w, h);
    }

	sf::View getLetterboxView(sf::View view, const int& windowWidth, const int& windowHeight) {
		float windowRatio = windowWidth / (float)windowHeight;
		float viewRatio = view.getSize().x / (float)view.getSize().y;
		float sizeX = 1;
		float sizeY = 1;
		float posX = 0;
		float posY = 0;

		bool horizontalSpacing = true;
		if (windowRatio < viewRatio)
			horizontalSpacing = false;

		if (horizontalSpacing) {
			sizeX = viewRatio / windowRatio;
			posX = (1 - sizeX) / 2.0;
		}
		else {
			sizeY = windowRatio / viewRatio;
			posY = (1 - sizeY) / 2.0;
		}

		view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));

		return view;
	}
private:
	std::deque<sf::Vector2f> targets;
};