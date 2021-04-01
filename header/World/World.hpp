#pragma once

#include "World/Entity.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <box2d/b2_world.h>

namespace astro
{

class Entity;

class World : public sf::Drawable
{
public:
	World();

	void createPlayerShip(const sf::Vector2f& position);
	void createAsteroid(const sf::Vector2f& position);

	void update(float deltaTime);
	void fixedUpdate(float deltaTime);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	const int m_velocityIterations = 6;
	const int m_positionIterations = 2;
	
	b2World m_physicsWorld;

	std::vector<Entity> m_entities;
};

}
