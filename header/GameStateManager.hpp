#pragma once

#include "GameStates/GameState.hpp"

#include <memory>
#include <vector>
#include <queue>

namespace zfge
{

class GameStateManager
{
public:
	GameStateManager();

	template<typename S, typename ...Args>
	void push(Args&&... args);
	void pop();
	
	template<typename S, typename ...Args>
	void change(Args&&... args);

	void clear();

	GameState& peek();
	const GameState& peek() const;
	
	bool isEmpty() const;
	std::size_t getStatesCount() const;

	void update();

private:
	struct Action
	{
		enum class Type
		{
			None,
			Push,
			Pop,
			Change,
			Clear
		};

		Type type = Type::None;
		std::unique_ptr<GameState> state;
	};

	std::vector<std::unique_ptr<GameState>> m_states;
	std::queue<Action> m_actionQueue;
};

template<typename S, typename... Args>
void GameStateManager::push(Args&&... args)
{
	Action& action = m_actionQueue.emplace();
	action.type = Action::Type::Push;
	action.state = std::make_unique<S>(std::forward<Args>(args)...);
}

template<typename S, typename ...Args>
void GameStateManager::change(Args&&... args)
{
	Action& action = m_actionQueue.emplace();
	action.type = Action::Type::Change;
	action.state = std::make_unique<S>(std::forward<Args>(args)...);
}

}
