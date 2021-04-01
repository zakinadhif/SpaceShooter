#include "World/World.hpp"

#include "World/EntityFactory.hpp"

namespace astro
{

World::World()
	: m_physicsWorld({0,0})
{
	
}

void World::createPlayerShip(const sf::Vector2f &position)
{
	::astro::createPlayerShip(*this, position, &m_physicsWorld);
}

void World::update(float deltaTime)
{

}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

}

}
