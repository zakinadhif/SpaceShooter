#pragma once

#include "World/Builders/AsteroidBuilder.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <entt/entity/fwd.hpp>

namespace astro
{
	class World;

	void drawEntities(const entt::registry& registry, sf::RenderTarget& target);
	void clearShotAsteroids(entt::registry& registry);
	void clearCollidedBullets(entt::registry& registry);
	void spawnAsteroidsRandomly(World& world, AsteroidBuilder& builder, sf::FloatRect spawnArea);
	void displayComponentInspector(entt::registry& registry);
} // namespace astro
