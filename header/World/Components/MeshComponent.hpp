#pragma once

#include <SFML/Graphics/PrimitiveType.hpp>
#include <bits/c++config.h>

namespace sf
{
class Vertex;
}

namespace astro
{

struct MeshComponent
{
	sf::Vertex* vertices;
	std::size_t size;
	sf::PrimitiveType type;
};

} // namespace astro
