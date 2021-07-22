#pragma once

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>

namespace astro
{

b2Body* createShipBody(b2World* world, const b2Vec2& position);

}
