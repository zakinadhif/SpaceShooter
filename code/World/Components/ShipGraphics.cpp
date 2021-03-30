#include "World/Components/ShipGraphics.hpp"

#include "World/Entity.hpp"
#include "World/Components/Interfaces/PhysicsComponent.hpp"

#include <Thor/Resources/ResourceHolder.hpp>

namespace astro
{

ShipGraphics::ShipGraphics(thor::ResourceHolder<sf::Texture, std::string>& textureHolder, PhysicsComponent* physics)
	: m_physicsBody(physics->getBody())
{
	m_sprite.setTexture(textureHolder["ship"]);
}

void ShipGraphics::update(Entity &entity)
{
}

void ShipGraphics::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}

}

