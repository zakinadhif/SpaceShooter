#pragma once

#include <Utility/PerlinNoise.hpp>

#include <SFML/System/Vector2.hpp>
#include <Thor/Math/Triangulation.hpp>

namespace astro
{

template <typename VectorType>
using Triangle = std::array<VectorType, 3>;

using Triangle2f = Triangle<sf::Vector2f>;

std::vector<float> generateAsteroidHeights(std::size_t pointsCount, float scale, float perlinIncrement);
std::vector<sf::Vector2f> generateAsteroidOuterVertices(const std::vector<float>& heights, float baseHeight);
std::vector<Triangle2f> generateAsteroidTriangleVertices(const std::vector<sf::Vector2f>& outerVertices);

}
