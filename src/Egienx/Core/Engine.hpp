#pragma once

#include "GameStateManager.hpp"
#include <cassert>
#include <type_traits>

namespace enx
{

class Engine
{
public:
	Engine();

	template<class T>
	void attach();

	void run();

	~Engine();

private:
	void handleEvents();

	GameStateManager m_gameStateManager;
	sf::RenderWindow m_window;
};

template <class T>
void Engine::attach()
{
	static_assert(
		std::is_constructible<T, GameStateManager&, sf::RenderTarget&>{},
		"GameState must be constructible with GameStateManager& and sf::RenderTarget&"
	);

	m_gameStateManager.push<T>(m_gameStateManager, m_window);
}

}
