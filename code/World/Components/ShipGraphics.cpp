#include "World/Components/ShipGraphics.hpp"

#include "World/Entity.hpp"
#include "World/Components/Interfaces/PhysicsComponent.hpp"
#include "Utility/VectorConverter.hpp"

#include <Thor/Resources/ResourceHolder.hpp>
#include <Thor/Math.hpp>
#include <box2d/box2d.h>

namespace astro
{

ShipGraphics::ShipGraphics(thor::ResourceHolder<sf::Texture, std::string>& textureHolder, PhysicsComponent* physics)
	: m_physicsBody(physics->getBody())
{
	const sf::Texture& shipTexture = textureHolder["ship"];

	m_sprite.setTexture(shipTexture);
}

void ShipGraphics::update(Entity &entity)
{
	sf::Vector2f shipPosition = b2Vec2ToSfVec(m_physicsBody->GetPosition());
	float shipRotation = thor::toDegree(m_physicsBody->GetAngle());

	m_sprite.setPosition(shipPosition);
	m_sprite.setRotation(shipRotation);
}

void ShipGraphics::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}

}

