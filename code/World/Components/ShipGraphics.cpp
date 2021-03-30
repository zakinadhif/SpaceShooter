#include "World/Components/ShipGraphics.hpp"
#include "World/Entity.hpp"

#include <Thor/Resources/ResourceHolder.hpp>

namespace astro
{

ShipGraphics::ShipGraphics(thor::ResourceHolder<sf::Texture, std::string>& textureHolder)
{
	m_sprite.setTexture(textureHolder["ship"]);
}

void ShipGraphics::update(Entity &entity)
{
	m_sprite.setPosition(entity.position);
}

void ShipGraphics::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}

}

