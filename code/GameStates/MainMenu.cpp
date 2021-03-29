#include "GameStates/MainMenu.hpp"

#include "GameStateManager.hpp"
#include "GameStates/PlayState.hpp"

namespace astro
{

MainMenu::MainMenu(zfge::GameStateManager& gameStateManager)
	: m_gameStateManager(gameStateManager)
	, m_startButton({225, 275}, {150, 50})
{
	m_startButton.setText("Start Game");
	m_startButton.onClick([this](){
		m_gameStateManager.push<PlayState>(m_gameStateManager);
	});
}

void MainMenu::handleEvent(sf::Event event)
{
	m_startButton.handleEvent(event);	

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

void MainMenu::update(float deltaTime) {}

void MainMenu::draw(sf::RenderTarget& target) const
{
	m_startButton.draw(target, sf::RenderStates::Default);
}

}
