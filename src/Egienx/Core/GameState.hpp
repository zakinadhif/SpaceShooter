#pragma once

#include <SFML/Graphics.hpp>

namespace enx
{

class GameState
{
public:
	GameState();

	virtual void update(float deltaTime);
	virtual void fixedUpdate(float deltaTime);

	virtual void handleEvent(sf::Event event);
	virtual void draw(sf::RenderTarget& target) const = 0;
	virtual void imGuiDraw();

	virtual ~GameState() = default;
};

}
