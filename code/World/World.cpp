#include "World/World.hpp"

#include "World/EntityFactory.hpp"

namespace astro
{

World::World()
	: m_physicsWorld({0,0})
{
	createPlayerShip({0,0});	
}

void World::createPlayerShip(const sf::Vector2f &position)
{
	Entity entity = ::astro::createPlayerShip(*this, position, &m_physicsWorld);

	m_entities.push_back(std::move(entity));
}

void World::handleEvent(const sf::Event& event)
{
	for (auto& entity : m_entities)
	{
		entity.handleEvent(event);
	}
}

void World::update(float deltaTime)
{
	for (auto& entity : m_entities)
	{
		entity.update(deltaTime);
	}
}

void World::fixedUpdate(float deltaTime)
{
	for (auto& entity : m_entities)
	{
		entity.fixedUpdate(deltaTime);
	}

	m_physicsWorld.Step(deltaTime, m_velocityIterations, m_positionIterations);
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::View lastView = target.getView();
	sf::View worldView({0.f, 0.f}, {12, 12});

	target.setView(worldView);

	for (auto& entity : m_entities)
	{
		entity.draw(target, states);
	}

	target.setView(lastView);
}

}
