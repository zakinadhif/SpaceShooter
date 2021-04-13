#pragma once

#include <SFML/System/Vector2.hpp>

namespace astro
{

struct EntityEvent
{
	enum class Direction { UP, DOWN, LEFT, RIGHT };
	
	enum class Type
	{
		Move,
		StopMove,
		PointTo
	};
	
	Type type;

	union
	{
		Direction direction;
		sf::Vector2f point;
	};
};

}
