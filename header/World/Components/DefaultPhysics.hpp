#pragma once

#include "Asteroid/PolygonsGenerator.hpp"
#include "World/Components/Interfaces/PhysicsComponent.hpp"

#include <SFML/System/Vector2.hpp>

namespace astro
{

class DefaultPhysics final : public PhysicsComponent
{
public:
	DefaultPhysics(b2Body* body) : m_body(body) {};

	void fixedUpdate(Entity& entity, float deltaTime) override {};
	void recieve(EntityEvent& event) override {};

	const b2Body* getBody() const override {return m_body;};

private:
	b2Body* m_body;
};

}
