#include "Asteroid/PolygonsGenerator.hpp"

#include "Utility/Random.hpp"

#include <vector>

namespace astro
{

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

}
