#include "World/Components/AsteroidGraphics.hpp"

#include "Utility/VectorConverter.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <box2d/box2d.h>
#include <Thor/Math.hpp>

namespace astro
{

AsteroidGraphics::AsteroidGraphics(const std::vector<sf::Vector2f>& outerVertices, PhysicsComponent* physics)
	: m_body(physics->getBody())
{
	m_shape.setPointCount(outerVertices.size());

	for (std::size_t x = 0; x < outerVertices.size(); ++x)
	{
		m_shape.setPoint(x, outerVertices[x]);
	}

	m_shape.setFillColor(sf::Color::White);
}

void AsteroidGraphics::update(Entity& entity)
{
	sf::Vector2f shipPosition = b2Vec2ToSfVec(m_body->GetPosition());
	float shipRotation = thor::toDegree(m_body->GetAngle());

	m_transformable.setPosition(shipPosition);
	m_transformable.setRotation(shipRotation);
}

void AsteroidGraphics::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= m_transformable.getTransform();
	target.draw(m_shape, states);
}

} // namespace astro
