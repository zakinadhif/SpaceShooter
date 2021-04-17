#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>

namespace astro
{

class CoordinateSpaceMapper
{
public:
	CoordinateSpaceMapper(sf::RenderTarget& target, const sf::View& view);

	sf::Vector2f mapToViewSpace(const sf::Vector2i& pixelPos) const;
	sf::Vector2i mapToWindowSpace(const sf::Vector2f& worldPos) const;

private:
	sf::RenderTarget& m_target;
	const sf::View& m_view;
};

}
