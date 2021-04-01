#pragma once

#include <SFML/Graphics.hpp>

namespace zfge
{

class GameState
{
public:
	GameState();

	virtual void update(float deltaTime);
	virtual void fixedUpdate(float deltaTime);

	virtual void handleEvent(sf::Event event);
	virtual void draw(sf::RenderTarget& target) const = 0;

	virtual ~GameState() = default;
};

}
