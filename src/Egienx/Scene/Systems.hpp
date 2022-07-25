#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <entt/entity/fwd.hpp>

namespace enx
{
	class Scene;

	void displayComponentInspector(entt::registry& registry);
	void displayEntityList(entt::registry& registry);
} // namespace enx
