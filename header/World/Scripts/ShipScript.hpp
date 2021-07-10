#pragma once

#include "World/CoordinateSpaceMapper.hpp"
#include "World/ScriptableEntity.hpp"
#include "World/Builders/BulletBuilder.hpp"

#include "box2d/b2_math.h"

#include <SFML/System/Clock.hpp>

class b2Body;

namespace astro
{

class World;
class Entity;

class ShipScript final : public ScriptableEntity
{
public:
	ShipScript(const CoordinateSpaceMapper& coordinateMapper, World& world, b2World& physicsWorld);

	void onCreate() override;
	void onDestroy() override;

	void onUpdate(float deltaTime) override;
	void onFixedUpdate(float deltaTime) override;

	void onEvent(sf::Event event) override;

private:
	b2Vec2 m_direction {};
	b2Vec2 m_pointToLookAt {};

	float m_maxSpeed = 5.f;
	float m_frictionConstant = 1.f;

	b2Body* m_body { nullptr };
	const CoordinateSpaceMapper* m_coordinateMapper { nullptr };

	float m_weaponReloadSpeed = 1.f;
	float m_weaponProjectileDamage = 1.f;

	float m_weaponTimer = 1.f;

	BulletBuilder m_bulletBuilder;

private:
	bool isTimeToShoot(float deltaTime);
	sf::Vector2f calculateBulletStartPosition() const;
};

} // namespace astro
