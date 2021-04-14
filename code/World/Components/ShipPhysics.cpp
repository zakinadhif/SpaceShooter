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

void ShipPhysics::fixedUpdate(Entity &entity, float deltaTime)
{
	b2Vec2 direction = sfVec2ToB2Vec(m_direction);

	const b2Vec2& shipLinearVelocity = m_body->GetLinearVelocity();
	b2Vec2 friction { -shipLinearVelocity.x * m_frictionConstant, -shipLinearVelocity.y *  m_frictionConstant };
	m_body->ApplyForceToCenter(friction, false);

	b2Vec2 force { direction.x * m_maxSpeed, direction.y * m_maxSpeed };
	m_body->ApplyForceToCenter(force, true);
}

void ShipPhysics::recieve(EntityEvent& event)
{
	switch (event.type)
	{
		case EntityEvent::Type::Move:
			handleMoveEvent(event.direction);
			break;
		case EntityEvent::Type::StopMove:
			handleStopEvent(event.direction);
			break;
		case EntityEvent::Type::PointTo:
			handlePointToEvent(event.point);
		default:
			break;
	}
}

void ShipPhysics::handlePointToEvent(const sf::Vector2f& point)
{
	
}

void ShipPhysics::handleMoveEvent(EntityEvent::Direction direction)
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
	
	if (m_direction != sf::Vector2f(0,0)) m_direction = thor::unitVector(m_direction);
}

void ShipPhysics::handleStopEvent(EntityEvent::Direction direction)
{
	using Direction = EntityEvent::Direction;

	switch (direction)
	{
		case Direction::UP:
			if (m_direction.y < 0)
				m_direction.y = 0;
			break;
		case Direction::DOWN:
			if (m_direction.y > 0)
				m_direction.y = 0;
			break;
		case Direction::LEFT:
			if (m_direction.x < 0)
				m_direction.x = 0;
			break;
		case Direction::RIGHT:
			if (m_direction.x > 0)
				m_direction.x = 0;
			break;
	}
	
	if (m_direction != sf::Vector2f(0,0)) m_direction = thor::unitVector(m_direction);
}

const b2Body* ShipPhysics::getBody() const
{
	return m_body;
}

}
