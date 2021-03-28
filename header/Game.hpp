#pragma once

#include "GameStateManager.hpp"

namespace astro
{

class Game
{
public:
	Game();

	void run();
private:
	void handleEvent(sf::Event event);
	void update(float deltaTime);
	void draw();

	zfge::GameStateManager m_gameStateManager;
	sf::RenderWindow m_window;
};

}
