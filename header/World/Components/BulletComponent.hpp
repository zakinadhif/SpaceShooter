#pragma once

#include <SFML/System/Vector2.hpp>

namespace astro
{

struct BulletComponent
{
	bool shouldBeDestroyed = false;
	float damage {};
};

} // namespace astro
