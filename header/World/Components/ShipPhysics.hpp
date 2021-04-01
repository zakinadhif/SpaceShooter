#pragma once

#include "World/Components/Interfaces/PhysicsComponent.hpp"

#include "World/EntityEvent.hpp"

#include <SFML/System/Vector2.hpp>

namespace astro
{

class ShipPhysics : public PhysicsComponent
{
public:
	ShipPhysics(b2Body* body);

	void fixedUpdate(Entity& entity, float deltaTime) override;

	void recieve(EntityEvent& event) override;

	const b2Body* getBody() const override;

private:
	void handleDirectionEvent(EntityEvent::Direction direction);

	sf::Vector2f m_direction;
	float m_moveForceMultiplier = 10.0f;

	b2Body* m_body;
};

}
