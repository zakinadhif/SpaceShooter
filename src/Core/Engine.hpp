#pragma once

#include "GameStateManager.hpp"

namespace enx
{

class Engine
{
public:
	Engine();

	template<class T>
	void pushGameState();

	void run();

	~Engine();

private:
	void handleEvents();

	enx::GameStateManager m_gameStateManager;
	sf::RenderWindow m_window;
};

}
