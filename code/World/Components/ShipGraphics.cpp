#include "World/Components/ShipGraphics.hpp"

#include "World/Entity.hpp"
#include "World/Components/Interfaces/PhysicsComponent.hpp"
#include "Utility/VectorConverter.hpp"

#include <Thor/Resources/ResourceHolder.hpp>
#include <Thor/Math.hpp>
#include <box2d/box2d.h>

namespace astro
{

ShipGraphics::ShipGraphics(PhysicsComponent* physics)
	: m_physicsBody(physics->getBody())
{
	m_model[0] = {{0.f, -0.5f}, sf::Color::White};
	m_model[1] = {{-0.5f, 0.5f}, sf::Color::White};
	m_model[2] = {{0.5f, 0.5f}, sf::Color::White};
}

void ShipGraphics::update(Entity &entity)
{
	sf::Vector2f shipPosition = b2Vec2ToSfVec(m_physicsBody->GetPosition());
	float shipRotation = thor::toDegree(m_physicsBody->GetAngle());

	m_transform.setPosition(shipPosition);
	m_transform.setRotation(shipRotation);
}

void ShipGraphics::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= m_transform.getTransform();
	target.draw(m_model.data(), 3, sf::Triangles, states);
}

}

