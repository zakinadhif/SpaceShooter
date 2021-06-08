#include "World/EntityFactory.hpp"

#include "World/World.hpp"
#include "World/Entity.hpp"
#include "World/PhysicsBodyFactory.hpp"

#include "Asteroid/PolygonsGenerator.hpp"
#include "Utility/VectorConverter.hpp"

#include "World/Components/ShipGraphics.hpp"
#include "World/Components/ShipInput.hpp"
#include "World/Components/ShipPhysics.hpp"
#include "World/Components/AsteroidGraphics.hpp"
#include "World/Components/DefaultPhysics.hpp"
#include "World/Components/NullInput.hpp"

#include <box2d/box2d.h>

namespace astro
{

Entity createPlayerShip(World& world, const sf::Vector2f &position, b2World* physicalWorld)
{
	b2Body* physicalShipBody = createShipBody(physicalWorld, sfVec2ToB2Vec(position));

	ShipPhysics* shipPhysics = new ShipPhysics(physicalShipBody);
	ShipGraphics* shipGraphics = new ShipGraphics(shipPhysics);
	ShipInput* shipInput = new ShipInput(world.getWorldSpaceMapper());

	return Entity(world, Entity::Type::Ship, shipInput, shipPhysics, shipGraphics);
}

Entity createAsteroid(World& world, const sf::Vector2f &position, b2World* physicalWorld)
{
	auto heights = generateAsteroidHeights(22, 0.5, 0.3);
	auto outerVertices = generateAsteroidOuterVertices<sf::Vector2f>(heights, 1);
	auto triangles = generateAsteroidTriangleVertices<b2Vec2, sf::Vector2f>(outerVertices);

	b2Body* body = createAsteroidBody(physicalWorld, sfVec2ToB2Vec(position), triangles);

	DefaultPhysics* physics = new DefaultPhysics(body);
	AsteroidGraphics* graphics = new AsteroidGraphics(outerVertices, physics);
	NullInput* input = new NullInput();

	return Entity(world, Entity::Type::Asteroid, input, physics, graphics);
}

}
