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
{
	createPlayerShip({0,0});

	auto asteroidHeights = generateAsteroidHeights(22, 0.5, 0.3);
	auto asteroidOuterVertices = generateAsteroidOuterVertices<sf::Vector2f>(
			asteroidHeights, 1);
	auto asteroidTriangles = generateAsteroidTriangleVertices<b2Vec2, sf::Vector2f>(asteroidOuterVertices);

	m_asteroid.setPointCount(asteroidOuterVertices.size());

	for (std::size_t x = 0; x < m_asteroid.getPointCount(); ++x)
	{
		m_asteroid.setPoint(x, asteroidOuterVertices[x]);
	}

	m_asteroid.setFillColor(sf::Color::White);
	m_asteroid.setPosition(0.f, 0.f);
}

const CoordinateSpaceMapper& World::getWorldSpaceMapper() const
{
	return m_worldSpaceMapper;
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

	static float ridgesScaleFactor = 0.5f;
	static float perlinIncrement = 0.3f;
	static float baseHeight = 1.0f;
	static int verticesCount = 22;
	static bool regenerateAsteroid = false;

	ImGui::Begin("Asteroid Generator");
	ImGui::InputFloat("Ridges Scale Factor", &ridgesScaleFactor);
	ImGui::InputFloat("Perlin Increment", &perlinIncrement);
	ImGui::InputFloat("Base Height", &baseHeight);
	ImGui::InputInt("Vertices Count", &verticesCount);

	regenerateAsteroid = ImGui::Button("Regenerate");
	ImGui::End();

	if (regenerateAsteroid)
	{
		auto asteroidHeights = generateAsteroidHeights(static_cast<std::size_t>(verticesCount), ridgesScaleFactor, perlinIncrement);
		auto asteroidOuterVertices = generateAsteroidOuterVertices<sf::Vector2f>(
				asteroidHeights, baseHeight);
		auto asteroidTriangles = generateAsteroidTriangleVertices<b2Vec2, sf::Vector2f>(asteroidOuterVertices);

		m_asteroid.setPointCount(asteroidOuterVertices.size());

		for (std::size_t x = 0; x < m_asteroid.getPointCount(); ++x)
		{
			m_asteroid.setPoint(x, asteroidOuterVertices[x]);
		}
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

	target.setView(m_worldView);

	for (auto& entity : m_entities)
	{
		entity.draw(target, states);
	}

	target.draw(m_asteroid, states);

	target.setView(lastView);
}

}
