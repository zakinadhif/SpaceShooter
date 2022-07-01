#pragma once

#include "World/ScriptableEntity.hpp"

#include "box2d/b2_math.h"

#include <SFML/System/Clock.hpp>

class b2Body;
class b2World;

namespace enx
{

class World;
class Entity;

class ShipScript final : public ScriptableEntity
{
public:
	ShipScript(World& world, b2World& physicsWorld);

	void onCreate() override;
	void onDestroy() override;

	void onUpdate(float deltaTime) override;
	void onFixedUpdate(float deltaTime) override;

	void onEvent(sf::Event event) override;
};

} // namespace enx
