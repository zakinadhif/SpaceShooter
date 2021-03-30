#pragma once

#include "box2d/b2_body.h"

namespace astro
{

class Entity;
class World;

class PhysicsComponent
{
public:
	virtual void update(Entity& entity, World& world) = 0;

	virtual const b2Body* getBody() const;
};

}
