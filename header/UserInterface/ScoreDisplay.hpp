#pragma once

#include <SFML/Graphics.hpp>

#include <entt/entity/fwd.hpp>

namespace astro
{

class ScoreDisplay : public sf::Drawable
{
public:
	ScoreDisplay();

	void setFont(sf::Font& font);

	void update(entt::registry& registry);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	float m_score;

	sf::Text m_text;
};

}
