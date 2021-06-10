#pragma once

#include <SFML/Graphics/Color.hpp>
#include <box2d/b2_draw.h>

namespace astro
{

sf::Color b2ColorToSfColor(const b2Color& color);
b2Color sfColorToB2Color(const sf::Color& color);

} // namespace astro
