#pragma once

#include <box2d/b2_math.h>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

sf::Vector2f b2Vec2ToSfVec(const b2Vec2& vector);
sf::Vector3f b2Vec3ToSfVec(const b2Vec3& vector);

b2Vec2 sfVec2ToB2Vec(const sf::Vector2f& vector);
b2Vec3 sfVec3ToB2Vec(const sf::Vector3f& vector);
