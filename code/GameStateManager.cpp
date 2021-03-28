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

void GameStateManager::handleEvent(sf::Event event)
{
	assert(!m_states.empty());

	peek().handleEvent(event);
}

void GameStateManager::update(const sf::Time& time)
{
	assert(!m_states.empty());
	
	peek().update(time);
}

void GameStateManager::draw(sf::RenderTarget &target) const
{
	assert(!m_states.empty());

	peek().draw(target);
}

}
