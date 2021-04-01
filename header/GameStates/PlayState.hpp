#pragma once

#include "GameStates/GameState.hpp"

#include "GameStateManager.hpp"
#include "World/World.hpp"

namespace astro
{

class PlayState final : public zfge::GameState
{
public:
	PlayState(zfge::GameStateManager& gameStateManager);

	void handleEvent(sf::Event event) override;
	
	void update(float deltaTime) override;
	void fixedUpdate(float deltaTime) override;

	void draw(sf::RenderTarget& target) const override;

private:
	zfge::GameStateManager& m_gameStateManager;
	World m_world;
};

}
