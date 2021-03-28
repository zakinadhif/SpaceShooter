#pragma once

#include <SFML/Graphics.hpp>

namespace zfge
{

class GameState
{
public:
	GameState();

	virtual void handleEvent(sf::Event event);
	virtual void update(float deltaTime);
	virtual void draw(sf::RenderTarget& target) const = 0;

	virtual ~GameState() = default;
};

}
