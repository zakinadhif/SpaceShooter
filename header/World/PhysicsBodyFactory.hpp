#pragma once

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>

#include <vector>
#include <array>

namespace astro
{

b2Body* createShipBody(b2World* world, const b2Vec2& position);
b2Body* createAsteroidBody(b2World* world, const b2Vec2& position, const std::vector<std::array<b2Vec2, 3>>& triangles);

}
