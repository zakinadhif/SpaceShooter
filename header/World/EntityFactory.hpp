#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include <Thor/Resources/ResourceHolder.hpp>

class b2World;

namespace astro
{

class Entity;
class World;

Entity createPlayerShip(World& world, const sf::Vector2f& position, b2World* physicalWorld);
Entity createAsteroid(const sf::Vector2f& position);

}
