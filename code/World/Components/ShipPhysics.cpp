#include "World/Components/ShipPhysics.hpp"

#include "Utility/Keyboard.hpp"
#include "Utility/VectorConverter.hpp"
#include "World/EntityEvent.hpp"

#include <Thor/Vectors.hpp>
#include <SFML/System/Vector2.hpp>

namespace astro
{

ShipPhysics::ShipPhysics(b2Body* body)
	: m_body(body)
{

}

void ShipPhysics::update(Entity &entity)
{
	// Normalize m_direction setted by handleDirectionEvent
	m_direction = thor::unitVector(m_direction);

	m_body->ApplyForceToCenter(sfVec2ToB2Vec(m_direction *  m_moveForceMultiplier), true);

	// Reset direction vector, because handleDirectionEvent doesn't reset the
	// direction when key press is released
	m_direction = {};
}

void ShipPhysics::recieve(EntityEvent& event)
{
	switch (event.type)
	{
		case EntityEvent::Type::Direction:
			handleDirectionEvent(event.direction);
			break;
		default:
			break;
	}
}

void ShipPhysics::handleDirectionEvent(EntityEvent::Direction direction)
{
	using Direction = EntityEvent::Direction;

	switch (direction)
	{
		case Direction::UP:
			m_direction.y += -1.0f;
			break;
		case Direction::DOWN:
			m_direction.y += 1.0f;
			break;
		case Direction::LEFT:
			m_direction.x += -1.0f;
			break;
		case Direction::RIGHT:
			m_direction.x += 1.0f;
			break;
	}
}

const b2Body* ShipPhysics::getBody() const
{
	return m_body;
}

}
