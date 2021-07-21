#include "World/Builders/AsteroidBuilder.hpp"

#include "Utility/Random.hpp"
#include "World/Entity.hpp"
#include "World/World.hpp"
#include "World/UnitScaling.hpp"
#include "World/Components/OwningMeshComponent.hpp"
#include "World/Components/RigidBodyComponent.hpp"
#include "Utility/delaunator.hpp"

#include <box2d/box2d.h>
#include <Thor/Math.hpp>

#include <cstdint>

namespace astro
{

AsteroidBuilder::AsteroidBuilder(World& world, b2World& physicsWorld)
	: m_world(&world)
	, m_physicsWorld(&physicsWorld)
	, m_perlinNoise(std::random_device{}())
{
}

Entity AsteroidBuilder::spawn()
{
	return createAsteroid();
}

Entity AsteroidBuilder::createAsteroid()
{
	auto entity = m_world->createEntity();

	const auto asteroidHeights = generateAsteroidHeights();
	const auto asteroidOuterVertices = generateAsteroidOuterVertices(asteroidHeights);
	const auto asteroidTriangles = generateAsteroidTriangleVertices(asteroidOuterVertices);

	entity.addComponent<RigidBodyComponent>(createAsteroidBody(asteroidTriangles));

	// Generate asteroid graphical vertices
	std::vector<sf::Vertex> vertices(asteroidTriangles.size() * 3);

	for (std::size_t x = 0; x < asteroidTriangles.size(); ++x)
	{
		sf::Color color = zfge::Random::getColor();
		vertices[x * 3]     = {asteroidTriangles[x][0], color};
		vertices[x * 3 + 1] = {asteroidTriangles[x][1], color};
		vertices[x * 3 + 2] = {asteroidTriangles[x][2], color};
	}

	auto& mesh = entity.addComponent<OwningMeshComponent>();
	mesh.type = sf::Triangles;
	mesh.vertices.swap(vertices);

	return entity;
}

b2Body* AsteroidBuilder::createAsteroidBody(const std::vector<Triangle2f>& triangles)
{
	std::vector<Triangleb2d> b2triangles;
	b2triangles.resize(triangles.size());

	for (std::size_t x = 0; x < triangles.size(); ++x)
	{
		b2triangles[x][0] = toMeters(triangles[x][0]);
		b2triangles[x][1] = toMeters(triangles[x][1]);
		b2triangles[x][2] = toMeters(triangles[x][2]);
	}

	const b2Vec2 position = toMeters(m_position);
	const b2Vec2 linearVelocity = toMeters(m_linearVelocity);

	const float angularVelocity = m_angularVelocity;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.angularVelocity = angularVelocity;
	bodyDef.linearVelocity = linearVelocity;

	b2Body* body = m_physicsWorld->CreateBody(&bodyDef);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = nullptr;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.3f;

	for (const auto& triangle : b2triangles)
	{
		b2PolygonShape box2dTriangle;
		box2dTriangle.Set(triangle.data(), 3);

		fixtureDef.shape = &box2dTriangle;

		body->CreateFixture(&fixtureDef);
	}

	return body;
}

std::vector<float> AsteroidBuilder::generateAsteroidHeights() const
{
	static const std::size_t pointsCount = 16;
	static const float perlinIncrement = 0.1f;
	static const float frequency = 8.f;
	static const std::int32_t octaves = 16;

	const double fx = pointsCount / frequency;

	std::vector<float> asteroidHeights(pointsCount);

	for (auto& height : asteroidHeights)
	{
		height = toPixels(m_perlinNoise.normalizedOctaveNoise1D(m_perlinCounter, octaves));
		height = zfge::Random::getFloat(10, 48);
		m_perlinCounter += perlinIncrement;
	}

	return asteroidHeights;
}

std::vector<sf::Vector2f> AsteroidBuilder::generateAsteroidOuterVertices(const std::vector<float>& heights) const
{
	std::vector<sf::Vector2f> vertices(heights.size());

	const float angleIncrement = 360.f / heights.size();
	float angle = 0.0f;

	for (std::size_t x = 0; x < vertices.size(); ++x)
	{
		float v_cos = std::cos(thor::toRadian(angle));
		float v_sin = std::sin(thor::toRadian(angle));

		vertices[x] = {v_cos * (heights[x] + m_baseHeight), v_sin * (heights[x] + m_baseHeight)};

		angle += angleIncrement;
	}

	return vertices;
}

std::vector<Triangle2f> AsteroidBuilder::generateAsteroidTriangleVertices(const std::vector<sf::Vector2f>& outerVertices) const
{
	std::vector<double> unpackedCoords(outerVertices.size() * 2u);

	for (std::size_t x = 0; x < outerVertices.size(); ++x)
	{
		unpackedCoords[x*2] = outerVertices[x].x;
		unpackedCoords[x*2 + 1] = outerVertices[x].y;
	}

	delaunator::Delaunator delaunator(unpackedCoords);

	std::vector<Triangle<sf::Vector2f>> triangles;
	triangles.reserve(delaunator.coords.size() / (3u*2u));

	for (std::size_t x = 0; x < delaunator.triangles.size(); x += 3)
	{
		sf::Vector2f t1, t2, t3;

		t1.x = delaunator.coords[2 * delaunator.triangles[x]];
		t1.y = delaunator.coords[2 * delaunator.triangles[x] + 1];

		t2.x = delaunator.coords[2 * delaunator.triangles[x + 1]];
		t2.y = delaunator.coords[2 * delaunator.triangles[x + 1] + 1];

		t3.x = delaunator.coords[2 * delaunator.triangles[x + 2]];
		t3.y = delaunator.coords[2 * delaunator.triangles[x + 2] + 1];

		triangles.push_back({t1, t2, t3});
	}

	return triangles;
}

} // namespace astro
