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
	void handleEvent();

	zfge::GameStateManager m_gameStateManager;
	sf::RenderWindow m_window;
};

}
