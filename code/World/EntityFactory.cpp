#include "World/EntityFactory.hpp"

#include "World/World.hpp"
#include "World/Entity.hpp"
#include "World/PhysicsBodyFactory.hpp"

#include "World/Components/ShipGraphics.hpp"
#include "World/Components/ShipInput.hpp"
#include "World/Components/ShipPhysics.hpp"

#include <box2d/box2d.h>

namespace astro
{

Entity createPlayerShip(World& world, const sf::Vector2f &position, b2World* physicalWorld)
{
	b2Body* physicalShipBody = createShipBody(physicalWorld);

	ShipPhysics* shipPhysics = new ShipPhysics(physicalShipBody);
	ShipGraphics* shipGraphics = new ShipGraphics(shipPhysics);
	ShipInput* shipInput = new ShipInput(world.getWorldSpaceMapper());

	return Entity(world, Entity::Type::Ship, shipInput, shipPhysics, shipGraphics);
}

Entity createAsteroid(const sf::Vector2f &position)
{

}

}
