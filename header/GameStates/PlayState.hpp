#pragma once

#include "GameStates/GameState.hpp"

#include "GameStateManager.hpp"

namespace astro
{

class PlayState final : public zfge::GameState
{
public:
	PlayState(zfge::GameStateManager& gameStateManager);

	void handleEvent(sf::Event event) override;
	void update(float deltaTime) override;
	void draw(sf::RenderTarget& target) const override;

private:
	zfge::GameStateManager& m_gameStateManager;
};

}
