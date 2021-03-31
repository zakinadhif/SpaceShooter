#pragma once

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>

#include <Thor/Shapes/ConcaveShape.hpp>

namespace astro
{

b2Body* createShipBody(b2World* world);
b2Body* createAsteroidBody(b2World* world, thor::ConcaveShape shape);

}
