#include "Scene/Scene.hpp"

#include "Utility/Box2dDebugDraw.hpp"
#include "Core/Random.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Systems.hpp"

#include "Scene/Components/Components.hpp"
#include "Scene/Components/RigidBodyComponent.hpp"
#include "Scene/Scripts/ShipScript.hpp"

#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>
#include <spdlog/spdlog.h>
#include <imgui.h>

#include <iostream>
#include <unordered_map>

namespace enx
{

template<typename... Component>
static void copyComponent(entt::registry& dst, entt::registry& src, const std::unordered_map<IDComponent::IDType, entt::entity>& enttMap)
{
	([&]()
	{
		auto view = src.view<Component>();
		for (auto srcEntity : view)
		{
			entt::entity dstEntity = enttMap.at(src.get<IDComponent>(srcEntity).id);

			auto& srcComponent = src.get<Component>(srcEntity);
			dst.emplace_or_replace<Component>(dstEntity, srcComponent);
		}
	}(), ...);
}

template<typename... Component>
static void copyComponent(
		ComponentGroup<Component...>,
		entt::registry& dst,
		entt::registry& src,
		const std::unordered_map<IDComponent::IDType, entt::entity>& enttMap
		)
{
	copyComponent<Component...>(dst, src, enttMap);
}

Scene::Scene(sf::RenderTarget& mainWindow)
	: m_physicsWorld({0,0})
	, m_worldView(mainWindow.getDefaultView())
	, m_mainWindow(mainWindow)
	, m_box2dDebugDraw(mainWindow)
{
	m_registry.ctx().emplace<GameStateComponent>();

	m_munroFont.loadFromFile("assets/munro.ttf");

	m_physicsWorld.SetDebugDraw(&m_box2dDebugDraw);

	m_worldView.setCenter(0,0);

	m_registry.on_destroy<NativeScriptComponent>().connect<&Scene::deallocateNscInstance>();
	m_registry.on_destroy<RigidBodyComponent>().connect<&Scene::deallocateB2BodyInstance>();
}

std::unique_ptr<Scene> Scene::clone(Scene& other)
{
	std::unique_ptr<Scene> newScene = std::make_unique<Scene>(m_mainWindow);
	newScene->m_lastEntityId = m_lastEntityId;

	auto& srcSceneRegistry = other.m_registry;
	auto& dstSceneRegistry = newScene->m_registry;
	std::unordered_map<IDComponent::IDType, entt::entity> enttMap;

	// Create entities in new scene
	auto idView = srcSceneRegistry.view<IDComponent>();
	for (auto e : idView)
	{
		using IDType = IDComponent::IDType;

		IDType id = srcSceneRegistry.get<IDComponent>(e).id;
		const auto& name = srcSceneRegistry.get<TagComponent>(e).tag;
		Entity newEntity = newScene->createEntityWithID(id, name);
		enttMap[id] = (entt::entity) newEntity;
	}

	copyComponent(AllComponents{}, dstSceneRegistry, srcSceneRegistry, enttMap);

	return newScene;
}

Entity Scene::createEntity(const std::string& name)
{
	return createEntityWithID(++m_lastEntityId, name);
}

Entity Scene::createEntityWithID(IDComponent::IDType id, const std::string& name)
{
	Entity entity = { m_registry.create(), m_registry };
	entity.addComponent<IDComponent>(id);
	entity.addComponent<TransformComponent>();
	auto& tag = entity.addComponent<TagComponent>();

	tag.tag = name.empty() ? std::string("Entity ") + std::to_string(id) : name;
	return entity;
}

void Scene::restartPhysics()
{

}

void Scene::handleEvent(const sf::Event& event)
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

void Scene::updateScripts(float deltaTime)
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
}

void Scene::fixedUpdateScripts(float deltaTime)
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
}

void Scene::fixedUpdatePhysics(float deltaTime)
{
	m_physicsWorld.Step(deltaTime, m_velocityIterations, m_positionIterations);
}

void Scene::drawEditorInterface()
{
	displayComponentInspector(m_registry);
	displayEntityList(m_registry);
}

void Scene::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::View lastView = target.getView();

	target.setView(m_worldView);

	drawEntities(m_registry, target);

	m_physicsWorld.DebugDraw();

	target.setView(lastView);
}

void Scene::deallocateNscInstance(entt::registry& registry, entt::entity entity)
{
	auto& nsc = registry.get<NativeScriptComponent>(entity);

	if (nsc.instance)
	{
		nsc.instance->onDestroy();
		nsc.destroyScript(&nsc);
	}
}

void Scene::deallocateB2BodyInstance(entt::registry& registry, entt::entity entity)
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

Scene::~Scene()
{
	m_registry.clear();
}

}
