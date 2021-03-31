#pragma once

#include "World/Components/Interfaces/PhysicsComponent.hpp"

namespace astro
{

class ShipPhysics : public PhysicsComponent
{
public:
	ShipPhysics(b2Body* body);

	void update(Entity& entity) override;

	const b2Body* getBody() const override;

private:
	b2Body* m_body;
};

}
