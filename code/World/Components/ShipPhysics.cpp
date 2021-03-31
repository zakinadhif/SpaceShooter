#include "World/Components/ShipPhysics.hpp"

namespace astro
{

ShipPhysics::ShipPhysics(b2Body* body)
	: m_body(body)
{

}

void ShipPhysics::update(Entity &entity)
{

}

const b2Body* ShipPhysics::getBody() const
{
	return m_body;
}

}
