#pragma once

#include "World/ScriptableEntity.hpp"

#include "box2d/b2_math.h"

class b2Body;

namespace astro
{

class ShipScript final : public ScriptableEntity
{
public:
	ShipScript();

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

	b2Body* body;
};

} // namespace astro
