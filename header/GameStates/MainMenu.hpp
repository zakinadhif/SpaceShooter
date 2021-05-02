#pragma once

#include "GameStates/GameState.hpp"

#include "GameStateManager.hpp"
#include "UserInterface/Widgets/Button.hpp"

namespace astro
{

class MainMenu final : public zfge::GameState
{
public:
	MainMenu(zfge::GameStateManager& gameStateManager, sf::RenderTarget& mainWindow);

	void handleEvent(sf::Event event) override;
	void update(float deltaTime) override;
	void draw(sf::RenderTarget& target) const override;

private:
	zfge::GameStateManager& m_gameStateManager;

	Button m_startButton;
	sf::RenderTarget& m_mainWindow;
};

}
