#include "Asteroid/PolygonsGenerator.hpp"
#include "Utility/Random.hpp"

#include <Thor/Math/Triangulation.hpp>
#include <cmath>

std::vector<float> generateAsteroidHeights(std::size_t pointsCount, float scale, float perlinIncrement)
{
	static siv::PerlinNoise perlinNoise;

	std::vector<float> asteroidHeights(pointsCount);

	float x = 0.f;

	for (auto& height : asteroidHeights)
	{
		height = perlinNoise.noise1D(x) * scale;

		x += perlinIncrement;
	}

	return asteroidHeights;
}

std::vector<sf::Vector2f> generateAsteroidOuterVertices(const std::vector<float>& heights, float baseHeight)
{
	std::vector<sf::Vector2f> vertices(heights.size());

	const float angleIncrement = 360.f / heights.size();
	float angle = 0.0f;

	for (std::size_t x = 0; x < vertices.size(); ++x)
	{
		vertices[x] = {std::cos(angle) * heights[x] + baseHeight, std::sin(angle) * heights[x] + baseHeight};

		angle += angleIncrement;
	}

	return vertices;
}

std::vector<thor::Triangle<sf::Vector2f>> generateAsteroidTriangleVertices(const std::vector<sf::Vector2f>& outerVertices)
{
	std::vector<thor::Triangle<sf::Vector2f>> triangles;

	thor::triangulate(outerVertices.begin(), outerVertices.end(), triangles.begin());

	return triangles;
}
