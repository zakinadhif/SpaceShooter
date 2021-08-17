#include "World/Builders/BulletBuilder.hpp"

#include "World/Components/TransformComponent.hpp"
#include "World/World.hpp"
#include "World/UnitScaling.hpp"
#include "World/Entity.hpp"

#include "World/Components/MeshComponent.hpp"
#include "World/Components/BulletComponent.hpp"
#include "World/Components/RigidBodyComponent.hpp"

#include <box2d/box2d.h>
#include <cmath>

namespace astro
{

BulletBuilder::BulletBuilder(World& world, b2World& physicsWorld)
	: m_world(&world), m_physicsWorld(&physicsWorld)
{

}

void BulletBuilder::setSpeed(float speed)
{
	m_bulletSpeed = speed;
}

void BulletBuilder::setDamage(float damage)
{
	m_bulletDamage = damage;
}

void BulletBuilder::setSize(sf::Vector2f size)
{
	m_bulletSize = size;
}

void BulletBuilder::setPosition(sf::Vector2f position)
{
	m_bulletPosition = position;
}

void BulletBuilder::setAngle(float angle)
{
	m_bulletAngle = angle;
}

Entity BulletBuilder::spawn()
{
	static std::array<sf::Vertex, 4> rectangleMesh {
		{
			{{-0.5f, -0.5f}, sf::Color::White},
			{{-0.5f, 0.5f}, sf::Color::White},
			{{0.5f, 0.5f}, sf::Color::White},
			{{0.5f, -0.5f}, sf::Color::White}
		}
	};

	b2Body* body = createBulletBody(m_physicsWorld);

	Entity bullet = m_world->createEntity();

	bullet.addComponent<MeshComponent>(rectangleMesh.data(), rectangleMesh.size(), sf::Quads);
	bullet.addComponent<BulletComponent>(m_bulletDamage);

	auto& rgc = bullet.addComponent<RigidBodyComponent>(body);
	rgc.body->GetUserData().pointer = (uintptr_t) new Entity(bullet);

	auto& tc = bullet.addComponent<TransformComponent>();
	tc.setScale(m_bulletSize);

	return bullet;
}

b2Body* BulletBuilder::createBulletBody(b2World* world) const
{
	const b2Vec2 position = toMeters(m_bulletPosition);
	const b2Vec2 size = toMeters(m_bulletSize);

	const float speed = toMeters(m_bulletSpeed);
	const b2Vec2 velocity {std::cos(m_bulletAngle) * speed, std::sin(m_bulletAngle) * speed};

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.bullet = true;
	
	bodyDef.position.Set(position.x, position.y);
	bodyDef.angle = m_bulletAngle +  1.57079632679f;

	bodyDef.linearVelocity = velocity;

	b2Body* body = world->CreateBody(&bodyDef);

	b2PolygonShape rectangleShape;
	rectangleShape.SetAsBox(size.x / 2, size.y / 2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &rectangleShape;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.3f;

	body->CreateFixture(&fixtureDef);

	return body;
}

} // namespace astro
