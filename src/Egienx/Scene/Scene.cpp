#include "Scene/Scene.hpp"

#include "Scene/Entity.hpp"
#include "Scene/Systems.hpp"
#include "Scene/Components/Components.hpp"

#include <entt/entity/fwd.hpp>
#include <entt/entt.hpp>
#include <spdlog/spdlog.h>
#include <imgui.h>
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
	: m_worldView(mainWindow.getDefaultView())
	, m_mainWindow(mainWindow)
{
	m_registry.ctx().emplace<GameStateComponent>();

	m_nscDeallocatorConnection = m_registry.on_destroy<NativeScriptComponent>().connect<&Scene::deallocateNscInstance>();
}

// Clones self to a new instance of Scene.
//
// NOTE: cloned scene won't have its physics initialized.
std::unique_ptr<Scene> Scene::clone(Scene& other)
{
	std::unique_ptr<Scene> newScene = std::make_unique<Scene>(other.m_mainWindow);
	newScene->m_lastEntityId = other.m_lastEntityId;

	// Prepare source and destination.
	// enttMap is used to correctly initalize entities in its place (the id).
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

// Creates entity with automatically generated IDComponent::id
// using increments based on m_lastEntityId. This method would
// be used in runtime. See @createEntityWithID if you want to
// initailize entities based on savefile.
//
// NOTE: To work properly post-deserialization from savefile,
//       m_lastEntityId should be assigned with last value
//       used.
Entity Scene::createEntity(const std::string& name)
{
	return createEntityWithID(++m_lastEntityId, name);
}

// Creates new entity with proper initialization.
// Entity should be instantiated with IDComponent, TransformComponent
// and TagComponent attached to it to help the user differentiates
// entities in EntityInspectorPanel
Entity Scene::createEntityWithID(IDComponent::IDType id, const std::string& name)
{
	Entity entity = { m_registry.create(), m_registry };
	entity.addComponent<IDComponent>(id);
	entity.addComponent<TransformComponent>();
	auto& tag = entity.addComponent<TagComponent>();

	tag.tag = name.empty() ? std::string("Entity ") + std::to_string(id) : name;
	return entity;
}

// Dispatch events to NativeScript components
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

// Instantiate newly created NativeScriptComponent internals and calls its
// onUpdate method
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

// Calls NativeScriptComponents' onFixedUpdate method
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

void Scene::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::View lastView = target.getView();

	target.setView(m_worldView);


	target.setView(lastView);
}

// An entt event-driven function that deallocates NativeScript instances
// automatically when the component it is contained in is destroyed.
void Scene::deallocateNscInstance(entt::registry& registry, entt::entity entity)
{
	auto& nsc = registry.get<NativeScriptComponent>(entity);

	if (nsc.instance)
	{
		nsc.instance->onDestroy();
		nsc.destroyScript(&nsc);
	}
}

Scene::~Scene()
{
	m_registry.clear();
}

}
