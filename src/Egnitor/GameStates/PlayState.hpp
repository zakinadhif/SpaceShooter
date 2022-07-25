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

	void setScenePlay();
	void setSceneSimulate();
	void setSceneStop();

	void handleEvent(sf::Event event) override;

	void update(float deltaTime) override;
	void fixedUpdate(float deltaTime) override;

	void draw(sf::RenderTarget& target) const override;
	void imGuiDraw() override;

private:
	enx::GameStateManager& m_gameStateManager;

	std::unique_ptr<Scene> m_activeScene;
	std::unique_ptr<Scene> m_editorScene;

	enum class SceneState { Edit = 0, Simulate, Runtime };
	SceneState m_sceneState = SceneState::Simulate;
};

}
