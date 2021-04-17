#include "World/Components/ShipPhysics.hpp"

#include "Utility/Keyboard.hpp"
#include "Utility/VectorConverter.hpp"
#include "World/EntityEvent.hpp"

#include <Thor/Vectors.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <imgui.h>

namespace astro
{

ShipPhysics::ShipPhysics(b2Body* body)
	: m_body(body)
{

}

void ShipPhysics::fixedUpdate(Entity &entity, float deltaTime)
{
	const b2Vec2& shipLinearVelocity = m_body->GetLinearVelocity();
	const b2Vec2& shipPosition = m_body->GetPosition();
	float shipAngularVelocity = m_body->GetAngularVelocity();
	float shipAngle = m_body->GetAngle();

	b2Vec2 friction { -shipLinearVelocity.x * m_frictionConstant, -shipLinearVelocity.y *  m_frictionConstant };
	m_body->ApplyForceToCenter(friction, false);

	b2Vec2 force { m_direction.x * m_maxSpeed, m_direction.y * m_maxSpeed };
	m_body->ApplyForceToCenter(force, true);

	b2Vec2 lookAtVector = m_pointToLookAt - shipPosition;
	float desiredAngle = std::atan2(lookAtVector.y, lookAtVector.x) + thor::toRadian(90.f);

	m_body->SetTransform(shipPosition, desiredAngle);

	ImGui::Begin("ShipInfo");
	ImGui::LabelText("Rotation", "%f", desiredAngle);
	ImGui::End();
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
	m_pointToLookAt = sfVec2ToB2Vec(point);
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
	
	if (m_direction != b2Vec2(0,0)) m_direction.Normalize();
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

	if (m_direction != b2Vec2(0,0)) m_direction.Normalize();
}

const b2Body* ShipPhysics::getBody() const
{
	return m_body;
}

}
