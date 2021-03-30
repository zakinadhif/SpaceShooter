#include "Utility/VectorConverter.hpp"

sf::Vector2f b2Vec2ToSfVec(const b2Vec2& vector)
{
	return {vector.x, vector.y};
}

sf::Vector3f b2Vec3ToSfVec(const b2Vec3& vector)
{
	return {vector.x, vector.y, vector.z};
}

b2Vec2 sfVec2ToB2Vec(const sf::Vector2f& vector)
{
	return {vector.x, vector.y};
}

b2Vec3 sfVec3ToB2Vec(const sf::Vector3f& vector)
{
	return {vector.x, vector.y, vector.z};
}
