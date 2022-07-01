#include "Utility/Random.hpp"

#include <random>
#include <cassert>

namespace zfge::Random
{
	static std::uniform_int_distribution<int> uniform_int_distribution;
	static std::uniform_real_distribution<float> uniform_float_distribution;
	static std::bernoulli_distribution bernoulli_distribution;

	bool getBool(double chanceToGetTrue = 0.5)
	{
		assert(chanceToGetTrue >= 0 && chanceToGetTrue <= 1 && "getBool() chanceToGetTrue parameter must be positive");

		typedef decltype(bernoulli_distribution)::param_type param_type;
		return bernoulli_distribution(getEngine(), param_type(chanceToGetTrue));
	}

	float getFloat()
	{
		return uniform_float_distribution(getEngine());
	}

	int getInt()
	{
		return uniform_int_distribution(getEngine());
	}

	float getFloat(float min, float max)
	{
		typedef decltype(uniform_float_distribution)::param_type param_type;
		return uniform_float_distribution(getEngine(), param_type(min, max));
	}

	int getInt(int min, int max)
	{
		typedef decltype(uniform_int_distribution)::param_type param_type;
		return uniform_int_distribution(getEngine(), param_type(min, max));
	}

	sf::Color getColor()
	{
		return sf::Color(getInt(0, 255), getInt(0,255), getInt(0,255));
	}

	std::mt19937& getEngine()
	{
		static std::mt19937* mt = new std::mt19937(std::random_device{}());

		return *mt;
	}
}
