#pragma once

#include "Core/GameState.hpp"
#include "Core/GameStateManager.hpp"

#include "Scene/Scene.hpp"

namespace enx
{

class PlayState final : public enx::GameState
{
public:
	PlayState(enx::GameStateManager& gameStateManager, sf::RenderTarget& mainWindow);

	void handleEvent(sf::Event event) override;

	void update(float deltaTime) override;
	void fixedUpdate(float deltaTime) override;

	void draw(sf::RenderTarget& target) const override;

private:
	enx::GameStateManager& m_gameStateManager;
	Scene m_world;
};

}
