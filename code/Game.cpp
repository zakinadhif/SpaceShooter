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
	sf::Clock timer;
	sf::Time elapsed = sf::Time::Zero;

	while (m_window.isOpen() && !m_gameStateManager.isEmpty())
	{
		elapsed = timer.restart();

		for (sf::Event event; m_window.pollEvent(event);)
		{
			handleEvent(event);
		}

		update(elapsed.asSeconds());

		m_window.clear();
		draw();
		m_window.display();

		m_gameStateManager.tryPop();
	}
}

void Game::handleEvent(sf::Event event)
{
	m_gameStateManager.handleEvent(event);
}

void Game::update(float deltaTime)
{
	m_gameStateManager.update(deltaTime);
}

void Game::draw()
{
	m_gameStateManager.draw(m_window);
}

}
