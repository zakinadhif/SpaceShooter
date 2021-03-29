#pragma once

namespace astro
{

class Entity;
class World;

class PhysicsComponent
{
public:
	virtual void update(Entity& entity, World& world) = 0;
};

}
