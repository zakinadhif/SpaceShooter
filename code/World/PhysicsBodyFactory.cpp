#include "World/PhysicsBodyFactory.hpp"

#include <box2d/box2d.h>

namespace astro
{

b2Body* createShipBody(b2World* world, const b2Vec2& position)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);

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

}
