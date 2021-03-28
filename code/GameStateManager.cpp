#include "GameStateManager.hpp"

#include <cassert>

namespace zfge
{

GameStateManager::GameStateManager()
{

}

GameState& GameStateManager::peek()
{
	return *(m_states.top().get());
}

const GameState& GameStateManager::peek() const
{
	return *(m_states.top().get());
}

bool GameStateManager::isEmpty() const
{
	return m_states.empty();
}

std::size_t GameStateManager::getStatesCount() const
{
	return m_states.size();
}

void GameStateManager::pop()
{
	m_states.pop();
}

void GameStateManager::safePop()
{
	m_shouldPop = true;
}

void GameStateManager::tryPop()
{
	if (m_shouldPop) 
	{
		m_shouldPop = false;
		pop();
	}
}

void GameStateManager::handleEvent(sf::Event event)
{
	assert(!m_states.empty());

	peek().handleEvent(event);
}

void GameStateManager::update(float deltaTime)
{
	assert(!m_states.empty());
	
	peek().update(deltaTime);
}

void GameStateManager::draw(sf::RenderTarget &target) const
{
	assert(!m_states.empty());

	peek().draw(target);
}

}
