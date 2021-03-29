#pragma once

#include <SFML/Graphics/Drawable.hpp>

namespace astro
{

class GraphicsComponent : public sf::Drawable
{
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};

}
