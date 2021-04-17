#include "World/CoordinateSpaceMapper.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

namespace astro
{

CoordinateSpaceMapper::CoordinateSpaceMapper(sf::RenderTarget& target, const sf::View& view)
	: m_target(target), m_view(view)
{

}

sf::Vector2f CoordinateSpaceMapper::mapToViewSpace(const sf::Vector2i& pixelPos) const
{
	return m_target.mapPixelToCoords(pixelPos, m_view);
}

sf::Vector2i CoordinateSpaceMapper::mapToWindowSpace(const sf::Vector2f& worldPos) const
{
	return m_target.mapCoordsToPixel(worldPos, m_view);
}

}
