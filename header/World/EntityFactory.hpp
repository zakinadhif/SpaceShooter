#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>

#include <Thor/Resources/ResourceHolder.hpp>

namespace astro
{

typedef thor::ResourceHolder<sf::Texture, std::string> TextureHolder;

class Entity;

Entity createPlayerShip(const sf::Vector2f& position, TextureHolder& textureHolder);
Entity createAsteroid(const sf::Vector2f& position);

}
