#include "World/EntityFactory.hpp"

#include "Asteroid/PolygonsGenerator.hpp"
#include "Utility/VectorConverter.hpp"
#include "World/Components/Components.hpp"
#include "World/Components/RigidBodyComponent.hpp"
#include "World/PhysicsBodyFactory.hpp"
#include "World/UnitScaling.hpp"

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <box2d/box2d.h>
#include <entt/entt.hpp>

#include <array>

namespace astro
{

entt::entity spawnShip(entt::registry& registry, const sf::Vector2f& position, b2World* physicsWorld)
{
	static std::array<sf::Vertex, 3> shipMesh
	{
		{
			{{0.f * PPM, -0.5f * PPM}, sf::Color::White},
			{{-0.5f * PPM, 0.5f * PPM}, sf::Color::White},
			{{0.5f * PPM, 0.5f * PPM}, sf::Color::White}
		}
	};

	auto entity = registry.create();

	registry.emplace<MeshComponent>(entity, shipMesh.data(), shipMesh.size(), sf::Triangles);
	registry.emplace<RigidBodyComponent>(entity, createShipBody(physicsWorld, sfVec2ToB2Vec(position)));

	return entity;
}

entt::entity spawnAsteroid(entt::registry& registry, const sf::Vector2f& position, b2World* physicsWorld)
{
	auto entity = registry.create();

	const auto asteroidHeights = generateAsteroidHeights(22, 1.f, 0.3f);
	const auto asteroidOuterVertices = generateAsteroidOuterVertices<sf::Vector2f>(asteroidHeights, 1.f);
	const auto asteroidTriangles = generateAsteroidTriangleVertices<b2Vec2, sf::Vector2f>(asteroidOuterVertices);

	std::vector<sf::Vertex> vertices(asteroidTriangles.size() * 3);

	for (std::size_t x = 0; x < asteroidTriangles.size(); ++x)
	{
		vertices[x * 3]     = {{b2Vec2ToSfVec(asteroidTriangles[x][0])}, sf::Color::White};
		vertices[x * 3 + 1] = {{b2Vec2ToSfVec(asteroidTriangles[x][1])}, sf::Color::White};
		vertices[x * 3 + 2] = {{b2Vec2ToSfVec(asteroidTriangles[x][2])}, sf::Color::White};
	}

	registry.emplace<RigidBodyComponent>(entity, createAsteroidBody(physicsWorld, sfVec2ToB2Vec(position), asteroidTriangles));

	toPixels(vertices.data(), vertices.size());

	auto& mesh = registry.emplace<OwningMeshComponent>(entity);
	mesh.type = sf::Triangles;
	mesh.vertices.swap(vertices);

	return entity;
}

} // namespace astro
