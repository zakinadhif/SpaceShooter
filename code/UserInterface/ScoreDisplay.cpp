#include "UserInterface/ScoreDisplay.hpp"

#include "World/Components/GameStateComponent.hpp"

#include <entt/entt.hpp>
#include <fmt/format.h>

namespace astro
{

ScoreDisplay::ScoreDisplay()
{
	m_text.setCharacterSize(23);
}

void ScoreDisplay::setFont(sf::Font &font)
{
	m_text.setFont(font);
}

void ScoreDisplay::update(entt::registry& registry)
{
	auto& gameState = registry.ctx<GameStateComponent>();
	m_score = gameState.score;

	m_text.setString(fmt::format("SCORE {}", m_score));

	// Position the text
	auto screenWidth = gameState.screenWidth;
	auto screenHeight = gameState.screenHeight;

	auto textWidth = m_text.getLocalBounds().width;
	auto textHeight = m_text.getLocalBounds().height;

	auto textXPosition = (screenWidth - textWidth) / 2;
	auto textYPosition = (screenHeight - textHeight) / 2;

	m_text.setPosition(textXPosition, textYPosition);
}

void ScoreDisplay::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::View oldView = target.getView();
	target.setView(target.getDefaultView());
	target.draw(m_text);
	target.setView(oldView);
}

}
