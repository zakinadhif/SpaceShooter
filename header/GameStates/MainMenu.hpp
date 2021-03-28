#pragma once

#include "GameStates/GameState.hpp"
#include "GameStateManager.hpp"

namespace astro
{

class MainMenu final : public zfge::GameState
{
public:
	MainMenu(zfge::GameStateManager& gameStateManager);

	void handleEvent(sf::Event event) override;
	void update(const sf::Time& time) override;
	void draw(sf::RenderTarget& target) const override;

private:
	zfge::GameStateManager& m_gameStateManager;
};

}
