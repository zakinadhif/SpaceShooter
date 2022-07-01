#pragma once

#include "GameStates/GameState.hpp"

#include "GameStateManager.hpp"
#include "World/World.hpp"

namespace enx
{

class PlayState final : public zfge::GameState
{
public:
	PlayState(zfge::GameStateManager& gameStateManager, sf::RenderTarget& mainWindow);

	void handleEvent(sf::Event event) override;

	void update(float deltaTime) override;
	void fixedUpdate(float deltaTime) override;

	void draw(sf::RenderTarget& target) const override;

private:
	zfge::GameStateManager& m_gameStateManager;
	World m_world;
};

}
