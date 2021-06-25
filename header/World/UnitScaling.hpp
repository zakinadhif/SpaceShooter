#pragma once

#include <cstddef>

class b2Vec2;
namespace sf
{ 
	class Vertex;

	template<typename T> class Vector2;
	typedef Vector2<float> Vector2f;
}

namespace astro
{
	inline constexpr float PPM = 64.f;

	b2Vec2 toMeters(const sf::Vector2f& pixels);
	sf::Vector2f toPixels(const b2Vec2& meters);

	void toMeters(sf::Vertex* vertices, std::size_t size);
	void toPixels(sf::Vertex* vertices, std::size_t size);
}
