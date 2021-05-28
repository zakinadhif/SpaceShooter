#include "Asteroid/PolygonsGenerator.hpp"
#include "Utility/Random.hpp"

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <Thor/Math/Triangulation.hpp>
#include <Thor/Math/TriangulationFigures.hpp>

#include <Thor/Math/Trigonometry.hpp>
#include <cmath>
#include <vector>

namespace astro
{

namespace
{
	struct TriangleGenerator
	{
		TriangleGenerator(std::vector<Triangle2f>& triangles)
			: triangles(&triangles)
		{
			triangles.clear();
		}

		// Fake dereferencing
		TriangleGenerator& operator*()
		{
			return *this;
		}

		// Fake pre-increment and post-increment
		TriangleGenerator& operator++()
		{
			return *this;
		}

		TriangleGenerator& operator++(int)
		{
			return *this;
		}

		TriangleGenerator& operator=(const thor::Triangle<const sf::Vector2f>& triangle)
		{
			triangles->push_back({triangle[0], triangle[1], triangle[2]});
			
			return *this;
		}

		std::vector<Triangle2f>* triangles;
	};
}

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
		float v_cos = std::cos(thor::toRadian(angle));
		float v_sin = std::sin(thor::toRadian(angle));

		vertices[x] = {v_cos * (heights[x] + baseHeight), -v_sin * (heights[x] + baseHeight)};

		angle += angleIncrement;
	}

	return vertices;
}

std::vector<Triangle2f> generateAsteroidTriangleVertices(const std::vector<sf::Vector2f>& outerVertices)
{
	std::vector<Triangle2f> triangles;

	thor::triangulate(outerVertices.begin(), outerVertices.end(), TriangleGenerator(triangles));

	return triangles;
}

}
