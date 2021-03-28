#pragma once

#include "GameStates/GameState.hpp"

#include <SFML/Graphics.hpp>

#include <memory>
#include <stack>

namespace zfge
{

class GameStateManager
{
public:
	GameStateManager();

	template<typename S, typename... Args>
	void push(Args&&... args);
	void pop();
	GameState& peek();

	const GameState& peek() const;
	
	bool isEmpty() const;
	std::size_t getStatesCount() const;

	void handleEvent(sf::Event event);
	void update(const sf::Time& elapsed);
	void draw(sf::RenderTarget& target) const;

private:
	std::stack<std::unique_ptr<GameState>> m_states;
};

template<typename S, typename... Args>
void GameStateManager::push(Args&&... args)
{
	auto gameState = std::make_unique<S>(std::forward<Args>(args)...);
	m_states.push(std::move(gameState));
}

}
