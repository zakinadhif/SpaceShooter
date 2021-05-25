#pragma once

#include <Utility/PerlinNoise.hpp>

#include <SFML/System/Vector2.hpp>
#include <Thor/Math/Triangulation.hpp>

std::vector<float> generateAsteroidHeights(std::size_t pointsCount, float scale, float perlinIncrement);
std::vector<sf::Vector2f> generateAsteroidOuterVertices(const std::vector<float>& heights, float baseHeight);
std::vector<thor::Triangle<sf::Vector2f>> generateAsteroidTriangleVertices(const std::vector<sf::Vector2f>& outerVertices);
