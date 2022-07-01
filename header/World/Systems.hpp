#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <entt/entity/fwd.hpp>

namespace enx
{
	class World;

	void drawEntities(const entt::registry& registry, sf::RenderTarget& target);
	void displayComponentInspector(entt::registry& registry);
	void displayEntityList(entt::registry& registry);
} // namespace enx
