#pragma once

#include <SFML/Graphics/Drawable.hpp>

namespace astro
{

class Entity;

class GraphicsComponent : public sf::Drawable
{
	virtual void update(Entity& entity) = 0;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};

}
