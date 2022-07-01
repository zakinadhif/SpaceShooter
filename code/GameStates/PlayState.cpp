#include "GameStates/PlayState.hpp"

namespace enx
{

PlayState::PlayState(zfge::GameStateManager& gameStateManager, sf::RenderTarget& mainWindow)
	: m_gameStateManager(gameStateManager)
	, m_world(mainWindow)
{
}

void PlayState::handleEvent(sf::Event event)
{
	m_world.handleEvent(event);

	switch (event.type)
	{
		case sf::Event::KeyReleased:
			if (event.key.code == sf::Keyboard::Key::Escape)
				m_gameStateManager.pop();
			break;
		case sf::Event::Closed:
			m_gameStateManager.clear();
		default:
			break;
	}
}

void PlayState::update(float deltaTime)
{
	m_world.update(deltaTime);
}

void PlayState::fixedUpdate(float deltaTime)
{
	m_world.fixedUpdate(deltaTime);
}

void PlayState::draw(sf::RenderTarget& target) const
{
	m_world.draw(target, sf::RenderStates::Default);
}

}
