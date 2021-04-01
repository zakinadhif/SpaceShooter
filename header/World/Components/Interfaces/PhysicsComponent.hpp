#pragma once

#include "box2d/b2_body.h"

class b2Body;

namespace astro
{

class Entity;
class World;
class EntityEvent;

class PhysicsComponent
{
public:
	virtual void update(Entity& entity) = 0;
	virtual void recieve(EntityEvent& event) {};

	virtual const b2Body* getBody() const = 0;

	virtual ~PhysicsComponent() = default;
};

}
