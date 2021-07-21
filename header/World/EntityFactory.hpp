#pragma once

#include <SFML/System/Vector2.hpp>
#include <entt/entity/fwd.hpp>

class b2World;

namespace astro
{

entt::entity spawnShip(entt::registry& registry, const sf::Vector2f& position, b2World* physicsWorld);

} // namespace astro
