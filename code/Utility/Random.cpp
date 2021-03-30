#include "Utility/Random.hpp"

#include <random>

namespace zfge::Random
{
	static std::uniform_int_distribution<int> uniform_int_dist;
	static std::uniform_real_distribution<float> uniform_float_distribution;

	float getFloat()
	{
		return uniform_float_distribution(getEngine());
	}

	int getInt()
	{
		return uniform_int_dist(getEngine());
	}

	std::mt19937& getEngine()
	{
		static std::mt19937* mt = new std::mt19937(std::random_device{}());

		return *mt;
	}
}
