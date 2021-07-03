#pragma once

#include "GameStates/GameState.hpp"

#include "GameStateManager.hpp"
#include "UserInterface/Widgets/Button.hpp"
#include "UserInterface/Starfield.hpp"

#include <TGUI/Backends/SFML.hpp>

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

	Starfield m_starfield;

	mutable tgui::GuiSFML m_gui;

	sf::RenderTarget& m_mainWindow;

	void setVersionLabel(const std::string& versionString);
};

}
