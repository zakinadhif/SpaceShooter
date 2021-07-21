#pragma once

#include "Utility/PerlinNoise.hpp"

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>

#include <vector>

class b2World;
class b2Vec2;
class b2Body;

namespace astro
{

class World;
class Entity;

template <typename VectorType>
using Triangle = std::array<VectorType, 3>;

using Triangle2f = Triangle<sf::Vector2f>;
using Triangleb2d = Triangle<b2Vec2>;

class AsteroidBuilder
{
public:
	AsteroidBuilder(World& world, b2World& physicsWorld);

	Entity spawn();

	sf::Vector2f getPosition() const {return m_position;}
	sf::Vector2f getLinearVelocity() const {return m_linearVelocity;}

	float getAngularVelocity() const {return m_angularVelocity;}
	float getBaseHeight() const {return m_baseHeight;}

	void setPosition(sf::Vector2f position) {m_position = position;}
	void setLinearVelocity(sf::Vector2f linearVelocity) {m_linearVelocity = linearVelocity;}

	void setAngularVelocity(float angularVelocity) {m_angularVelocity = angularVelocity;}
	void setBaseHeight(float baseHeight) {m_baseHeight = baseHeight;}

private:
	World* m_world {nullptr};
	b2World* m_physicsWorld {nullptr};

	siv::PerlinNoise m_perlinNoise;
	mutable float m_perlinCounter = 0.f;

	std::vector<std::vector<sf::Vertex>> m_meshHolder;

	sf::Vector2f m_position;
	sf::Vector2f m_linearVelocity;

	float m_angularVelocity {};
	float m_baseHeight {};

	sf::Vector2f calculateNextAsteroidPosition() const;
	sf::Vector2f calculateNextAsteroidLinearVelocity() const;
	float calculateNextAsteroidAngularVelocity() const;
	float calculateNextAsteroidBaseHeight() const;

	std::vector<float> generateAsteroidHeights() const;
	std::vector<sf::Vector2f> generateAsteroidOuterVertices(const std::vector<float>& heights) const;
	std::vector<Triangle2f> generateAsteroidTriangleVertices(const std::vector<sf::Vector2f>& outerVertices) const;

	Entity createAsteroid();
	b2Body* createAsteroidBody(const std::vector<Triangle2f>& triangles);
};

} // namespace astro
