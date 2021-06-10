#include "Utility/ColorConverter.hpp"

namespace astro
{

sf::Color b2ColorToSfColor(const b2Color &color)
{
	return sf::Color(
		static_cast<sf::Uint8>(color.r * 255),
		static_cast<sf::Uint8>(color.g * 255),
		static_cast<sf::Uint8>(color.b * 255),
		static_cast<sf::Uint8>(color.a * 255)
	);
}

b2Color sfColorToB2Color(const sf::Color &color)
{
	return b2Color(
		color.r / 255.f,
		color.g / 255.f,
		color.b / 255.f,
		color.a / 255.f
	);
}

} // namespace astro
