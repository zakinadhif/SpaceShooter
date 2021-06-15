#pragma once

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include <vector>

struct OwningMeshComponent
{
	std::vector<sf::Vertex> vertices;
	sf::PrimitiveType type;
};
