#include "World/World.hpp"

#include "World/EntityFactory.hpp"
#include "Asteroid/PolygonsGenerator.hpp"

#include <imgui.h>
#include <spdlog/fmt/fmt.h>

#include <iostream>

template <>
struct fmt::formatter<b2Vec2>
{
	constexpr auto parse(format_parse_context& ctx)
	{
		auto it = ctx.begin(), end = ctx.end();
		if (it != end && *it != '}')
			throw format_error("Invalid sf::Vector2f format.");

		return it;
	}

	template <typename FormatContext>
	auto format(const b2Vec2& vec, FormatContext& ctx)
	{
		return format_to(ctx.out(), "{{{}, {}}}", vec.x, vec.y);
	}
};

namespace astro
{

World::World(sf::RenderTarget& mainWindow)
	: m_physicsWorld({0,0})
	, m_worldView({0.f, 0.f}, {12, 12})
	, m_mainWindow(mainWindow)
	, m_worldSpaceMapper(mainWindow, m_worldView)
	, m_box2dDebugDraw(mainWindow)
{
	m_physicsWorld.SetDebugDraw(&m_box2dDebugDraw);

	createPlayerShip({5,0});
	createAsteroid({0,0});
}

const CoordinateSpaceMapper& World::getWorldSpaceMapper() const
{
	return m_worldSpaceMapper;
}

void World::createPlayerShip(const sf::Vector2f &position)
{
	/* Entity entity = ::astro::createPlayerShip(*this, position, &m_physicsWorld);

	m_entities.push_back(std::move(entity)); */
}

void World::createAsteroid(const sf::Vector2f &position)
{
	/* Entity entity = ::astro::createAsteroid(*this, position, &m_physicsWorld);

	m_entities.push_back(std::move(entity)); */
}

void World::handleEvent(const sf::Event& event)
{
	/* for (auto& entity : m_entities)
	{
		entity.handleEvent(event);
	} */
}

void World::update(float deltaTime)
{
	/* for (auto& entity : m_entities)
	{
		entity.update(deltaTime);
	} */
}

void World::fixedUpdate(float deltaTime)
{
	/* for (auto& entity : m_entities)
	{
		entity.fixedUpdate(deltaTime);
	} */

	m_physicsWorld.Step(deltaTime, m_velocityIterations, m_positionIterations);
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::View lastView = target.getView();

	target.setView(m_worldView);

	/* for (auto& entity : m_entities)
	{
		entity.draw(target, states);
	} */

	m_physicsWorld.DebugDraw();

	// target.draw(m_asteroid, states);

	target.setView(lastView);
}

}
