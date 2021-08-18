#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <entt/entity/fwd.hpp>

namespace astro
{
	void drawEntities(const entt::registry& registry, sf::RenderTarget& target);
	void clearShotAsteroids(entt::registry& registry);
	void clearCollidedBullets(entt::registry& registry);
} // namespace astro
