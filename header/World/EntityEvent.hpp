#pragma once

namespace astro
{

struct EntityEvent
{
	enum class Direction { UP, DOWN, LEFT, RIGHT };

	union
	{
		Direction direction;
	};

	enum class Type
	{
		Direction
	};

	Type type;
};

}
