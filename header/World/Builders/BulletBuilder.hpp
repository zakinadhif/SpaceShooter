#pragma once

#include <SFML/System/Vector2.hpp>

class b2Body;
class b2World;

namespace astro
{

class World;
class Entity;

class BulletBuilder
{
public:
	BulletBuilder(World& world, b2World& physicsWorld);

	BulletBuilder& setSpeed(float speed);
	BulletBuilder& setDamage(float damage);

	BulletBuilder& setSize(sf::Vector2f size);

	BulletBuilder& setPosition(sf::Vector2f position);
	BulletBuilder& setAngle(float angle);

	Entity spawn();

private:
	sf::Vector2f m_bulletSize {10.f, 30.f};
	sf::Vector2f m_bulletPosition {0.f, 0.f};

	float m_bulletDamage {1.f};
	float m_bulletSpeed {32.f};
	float m_bulletAngle {0.f};

	World* m_world = nullptr;
	b2World* m_physicsWorld = nullptr;

	b2Body* createBulletBody(b2World* world) const;
};

}
