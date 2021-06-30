#include "World/UnitScaling.hpp"

#include "Utility/VectorConverter.hpp"

#include <box2d/b2_math.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Vertex.hpp>

namespace astro
{
	constexpr float PPM = 48.f;

	b2Vec2 toMeters(const sf::Vector2f& pixels)
	{
		const b2Vec2 inMeters {pixels.x / PPM, pixels.y / PPM};

		return inMeters;
	}

	sf::Vector2f toPixels(const b2Vec2& meters)
	{
		const sf::Vector2f inPixels {meters.x * PPM, meters.y * PPM};

		return inPixels;
	}

	b2Vec2 toMeters(float x, float y)
	{
		const b2Vec2 inMeters {x / PPM, y / PPM};

		return inMeters;
	}

	sf::Vector2f toPixels(float x, float y)
	{
		const sf::Vector2f inPixels {x * PPM, y * PPM};

		return inPixels;
	}

	float toMeters(float pixels)
	{
		return pixels / PPM;
	}

	float toPixels(float meters)
	{
		return meters * PPM;
	}

	void toMeters(sf::Vertex *vertices, std::size_t size)
	{
		for (std::size_t x = 0; x < size; ++x)
		{
			auto& vertex = *(vertices + x);
			vertex.position = b2Vec2ToSfVec(toMeters(vertex.position));
		}
	}

	void toPixels(sf::Vertex *vertices, std::size_t size)
	{
		for (std::size_t x = 0; x < size; ++x)
		{
			auto& vertex = *(vertices + x);
			vertex.position = toPixels(sfVec2ToB2Vec(vertex.position));
		}
	}
}
