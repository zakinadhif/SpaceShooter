#include "World/World.hpp"

#include "Utility/Box2dDebugDraw.hpp"
#include "Utility/Random.hpp"
#include "World/Entity.hpp"
#include "World/EntityFactory.hpp"
#include "World/Systems.hpp"

#include "World/Components/NativeScriptComponent.hpp"
#include "World/Components/RigidBodyComponent.hpp"
#include "World/Components/GameStateComponent.hpp"
#include "World/Scripts/ShipScript.hpp"

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>
#include <imgui.h>

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
	, m_worldView(mainWindow.getDefaultView())
	, m_mainWindow(mainWindow)
	, m_worldSpaceMapper(mainWindow, m_worldView)
	, m_box2dDebugDraw(mainWindow)
	, m_asteroidBuilder(*this, m_physicsWorld)
{
	m_registry.set<GameStateComponent>();

	m_munroFont.loadFromFile("assets/munro.ttf");
	m_scoreDisplay.setFont(m_munroFont);

	auto& gameState = m_registry.ctx<GameStateComponent>();
	gameState.screenHeight = m_mainWindow.getDefaultView().getSize().y;
	gameState.screenWidth = m_mainWindow.getDefaultView().getSize().x;

	m_physicsWorld.SetDebugDraw(&m_box2dDebugDraw);
	m_physicsWorld.SetContactListener(&m_contactListener);

	m_worldView.setCenter(0,0);

	m_registry.on_destroy<NativeScriptComponent>().connect<&World::deallocateNscInstance>();
	m_registry.on_destroy<RigidBodyComponent>().connect<&World::deallocateB2BodyInstance>();

	auto ship = Entity{ spawnShip(*this, {0, -2}, &m_physicsWorld), m_registry };
	auto& shipScript = ship.addComponent<NativeScriptComponent>();

	shipScript.bind<ShipScript>(m_worldSpaceMapper, *this, m_physicsWorld);

	m_asteroidBuilder.spawn();
}

const CoordinateSpaceMapper& World::getWorldSpaceMapper() const
{
	return m_worldSpaceMapper;
}

Entity World::createEntity()
{
	return Entity{ m_registry.create(), m_registry };
}

void World::handleEvent(const sf::Event& event)
{
	auto view = m_registry.view<NativeScriptComponent>();

	for (auto entity : view)
	{
		auto& nsc = view.get<NativeScriptComponent>(entity);

		if (nsc.instance)
		{
			nsc.instance->onEvent(event);
		}
	}
}

void World::update(float deltaTime)
{
	auto view = m_registry.view<NativeScriptComponent>();

	for (auto entity : view)
	{
		auto& nsc = view.get<NativeScriptComponent>(entity);

		if (!nsc.instance)
		{
			nsc.instance = nsc.instantiateScript();
			nsc.instance->m_entity = Entity{ entity, m_registry };
			nsc.instance->onCreate();
		}

		nsc.instance->onUpdate(deltaTime);
	}

	clearShotAsteroids(m_registry);
	clearCollidedBullets(m_registry);
	
	const double asteroidSpawnChance = 0.01f;
	if (zfge::Random::getBool(asteroidSpawnChance))
	{
		const auto size = m_worldView.getSize();
		const auto center = m_worldView.getCenter();
		const sf::FloatRect spawnArea
		{
			center.x - (size.x / 2),
			center.y - (size.y / 2),
			size.x,
			size.y
		};

		spawnAsteroidsRandomly(*this, m_asteroidBuilder, spawnArea);
	}

	displayComponentInspector(m_registry);
	m_scoreDisplay.update(m_registry);
}

void World::fixedUpdate(float deltaTime)
{
	auto view = m_registry.view<NativeScriptComponent>();

	for (auto entity : view)
	{
		auto& nsc = view.get<NativeScriptComponent>(entity);

		if (nsc.instance)
		{
			nsc.instance->onFixedUpdate(deltaTime);
		}
	}

	m_physicsWorld.Step(deltaTime, m_velocityIterations, m_positionIterations);
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::View lastView = target.getView();

	target.setView(m_worldView);

	drawEntities(m_registry, target);
	target.draw(m_scoreDisplay);

	m_physicsWorld.DebugDraw();

	target.setView(lastView);
}

void World::deallocateNscInstance(entt::registry& registry, entt::entity entity)
{
	auto& nsc = registry.get<NativeScriptComponent>(entity);

	if (nsc.instance)
	{
		nsc.instance->onDestroy();
		nsc.destroyScript(&nsc);
	}
}

void World::deallocateB2BodyInstance(entt::registry& registry, entt::entity entity)
{
	auto& rb = registry.get<RigidBodyComponent>(entity);

	if (rb.body)
	{
		if (rb.body->GetUserData().pointer)
		{
			delete (Entity*) rb.body->GetUserData().pointer;
			spdlog::info("A b2Body instance was destroyed with an userdata attached.");
		}
		else
		{
			spdlog::info("A b2Body instance was destroyed without an userdata attached.");
		}

		b2World* world = rb.body->GetWorld();
		world->DestroyBody(rb.body);
	}
}

World::~World()
{
	m_registry.clear();
}

}
