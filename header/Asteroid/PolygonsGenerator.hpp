#pragma once

#include "Utility/PerlinNoise.hpp"
#include "Utility/delaunator-header-only.hpp"

#include <SFML/System/Vector2.hpp>
#include <box2d/b2_math.h>
#include <Thor/Math.hpp>
#include <Thor/Vectors.hpp>

#include <vector>

namespace astro
{

template <typename VectorType>
using Triangle = std::array<VectorType, 3>;

using Triangle2f = Triangle<sf::Vector2f>;
using Triangleb2d = Triangle<b2Vec2>;

std::vector<float> generateAsteroidHeights(std::size_t pointsCount, float scale, float perlinIncrement);

template <typename OutputVectorType>
std::vector<OutputVectorType> generateAsteroidOuterVertices(const std::vector<float>& heights, float baseHeight)
{
	std::vector<OutputVectorType> vertices(heights.size());

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

template <typename OutputVectorType, typename InputVectorType>
std::vector<Triangle<OutputVectorType>> generateAsteroidTriangleVertices(const std::vector<InputVectorType>& outerVertices)
{
	std::vector<double> unpackedCoords(outerVertices.size() * 2u);

	for (std::size_t x = 0; x < outerVertices.size(); ++x)
	{
		unpackedCoords[x*2] = outerVertices[x].x;
		unpackedCoords[x*2 + 1] = outerVertices[x].y;
	}

	delaunator::Delaunator delaunator(unpackedCoords);

	std::vector<Triangle<OutputVectorType>> triangles(unpackedCoords.size() / (3u*2u));

	for (std::size_t x = 0; x < triangles.size(); ++x)
	{
		OutputVectorType t1, t2, t3;

		t1.x = delaunator.coords[x*6];
		t1.y = delaunator.coords[x*6 + 1];

		t2.x = delaunator.coords[x*6 + 2];
		t2.y = delaunator.coords[x*6 + 3];

		t3.x = delaunator.coords[x*6 + 4];
		t3.y = delaunator.coords[x*6 + 5];

		triangles[x] = {t1, t2, t3};
	}

	return triangles;
}

}
