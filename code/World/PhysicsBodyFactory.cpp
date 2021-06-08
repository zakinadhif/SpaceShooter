#include "World/PhysicsBodyFactory.hpp"

#include "Asteroid/PolygonsGenerator.hpp"

#include <box2d/box2d.h>

#include <vector>

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

b2Body* createAsteroidBody(b2World* world, const b2Vec2& position, const std::vector<Triangleb2d>& triangles)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);

	b2Body* body = world->CreateBody(&bodyDef);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = nullptr;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.3f;

	for (const auto& triangle : triangles)
	{
		b2PolygonShape box2dTriangle;
		box2dTriangle.Set(triangle.data(), 3);

		fixtureDef.shape = &box2dTriangle;

		body->CreateFixture(&fixtureDef);
	}

	return body;
}

}
