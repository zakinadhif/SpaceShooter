#include "World/UnitScaling.hpp"

#include "Utility/VectorConverter.hpp"

#include <box2d/b2_math.h>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Vertex.hpp>

namespace astro
{
	b2Vec2 toMeters(const sf::Vector2f& pixels)
	{
		const b2Vec2 inMeters {pixels.x / astro::PPM, pixels.y / astro::PPM};

		return inMeters;
	}

	sf::Vector2f toPixels(const b2Vec2& meters)
	{
		const sf::Vector2f inPixels {meters.x * astro::PPM, meters.y * astro::PPM};

		return inPixels;
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
