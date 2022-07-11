#include "GameStates/PlayState.hpp"

namespace enx
{

PlayState::PlayState(enx::GameStateManager& gameStateManager, sf::RenderTarget& mainWindow)
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
			break;
		default:
			break;
	}
}

void PlayState::update(float deltaTime)
{
	switch (m_sceneState)
	{
		case SceneState::Edit:
			break;
		case SceneState::Simulate:
			break;
		case SceneState::Runtime:
			m_world.updateScripts(deltaTime);
			break;
	}
	m_world.drawEditorInterface();
}

void PlayState::fixedUpdate(float deltaTime)
{
	switch (m_sceneState)
	{
		case SceneState::Edit:
			break;
		case SceneState::Simulate:
			m_world.fixedUpdatePhysics(deltaTime);
			break;
		case SceneState::Runtime:
			m_world.fixedUpdateScripts(deltaTime);
			m_world.fixedUpdatePhysics(deltaTime);
			break;
	}
}

void PlayState::draw(sf::RenderTarget& target) const
{
	m_world.draw(target, sf::RenderStates::Default);
}

}
