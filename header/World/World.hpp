#pragma once

#include "World/Entity.hpp"

#include <box2d/b2_world.h>

namespace astro
{

class World : public sf::Drawable
{
public:
	World();

	void createPlayerShip(const sf::Vector2f& position);
	void createAsteroid(const sf::Vector2f& position);

	void update(float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	b2World m_physicsWorld;

	std::vector<Entity> m_entities;
};

}
