#include "GameStateManager.hpp"

#include <cassert>

namespace zfge
{

GameStateManager::GameStateManager()
{

}

GameState& GameStateManager::peek()
{
	return *(m_states.back().get());
}

const GameState& GameStateManager::peek() const
{
	return *(m_states.back().get());
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
	Action& action = m_actionQueue.emplace();
	action.type = Action::Type::Pop;
}

void GameStateManager::clear()
{
	Action& action = m_actionQueue.emplace();
	action.type = Action::Type::Clear;
}

void GameStateManager::update()
{
	while (!m_actionQueue.empty())
	{
		Action& action = m_actionQueue.front();
		
		assert(action.type != Action::Type::None);

		switch (action.type)
		{
			case Action::Type::Push:
				m_states.push_back(std::move(action.state));
				break;
			case Action::Type::Pop:
				m_states.pop_back();
				break;
			case Action::Type::Change:
				m_states.pop_back();
				m_states.push_back(std::move(action.state));
				break;
			case Action::Type::Clear:
				m_states.clear();
				break;
			default:
				break;
		}

		m_actionQueue.pop();
	}
}

}
