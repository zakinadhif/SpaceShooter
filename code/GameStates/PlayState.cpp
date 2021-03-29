#include "GameStates/PlayState.hpp"

namespace astro
{

PlayState::PlayState(zfge::GameStateManager& gameStateManager)
	: m_gameStateManager(gameStateManager)
{
}

void PlayState::handleEvent(sf::Event event)
{
	switch (event.type)
	{
		case sf::Event::KeyReleased:
			if (event.key.code == sf::Keyboard::Key::Escape)
				m_gameStateManager.pop();
			break;
		default:
			break;
	}
}

void PlayState::update(float deltaTime) {}

void PlayState::draw(sf::RenderTarget& target) const
{
}

}
