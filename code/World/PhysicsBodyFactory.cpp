#include "World/PhysicsBodyFactory.hpp"

#include <box2d/box2d.h>

namespace astro
{

b2Body* createShipBody(b2World* world)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 0.0f);
	
	b2Body* body = world->CreateBody(&bodyDef);

	b2Vec2 shipShape[] =
	{
		{0.f, -0.5f},
		{-0.5f, 0.5f},
		{0.5f, 0.5f}
	};

	b2PolygonShape dynamicTriangle;
	dynamicTriangle.Set(shipShape, 3);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicTriangle;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.3f;

	body->CreateFixture(&fixtureDef);

	return body;
}

b2Body* createAsteroidBody(b2World* world, thor::ConcaveShape shape)
{

}

}
