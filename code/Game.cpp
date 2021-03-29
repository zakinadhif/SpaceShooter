#include "Game.hpp"

#include "GameStates/MainMenu.hpp"

namespace astro
{

Game::Game()
	: m_window(sf::VideoMode(600, 600), "SpaceShooter")
{
	m_gameStateManager.push<MainMenu>(m_gameStateManager);
}

void Game::run()
{
	m_gameStateManager.update();

	sf::Clock timer;
	sf::Time elapsed = sf::Time::Zero;

	while (m_window.isOpen() && !m_gameStateManager.isEmpty())
	{
		zfge::GameState& currentState = m_gameStateManager.peek();
		
		elapsed = timer.restart();

		for (sf::Event event; m_window.pollEvent(event);)
		{
			currentState.handleEvent(event);
		}

		currentState.update(elapsed.asSeconds());

		m_window.clear();
		currentState.draw(m_window);
		m_window.display();

		m_gameStateManager.update();
	}
}

}
