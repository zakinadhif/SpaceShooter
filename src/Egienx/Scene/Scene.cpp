#include "Scene/Scene.hpp"

#include "Patch/Thor.hpp"
#include "Scene/UnitScaling.hpp"
#include "Utility/Box2dDebugDraw.hpp"
#include "Core/Random.hpp"
#include "Core/Time.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Systems.hpp"

#include "Scene/Components/Components.hpp"
#include "Scene/Scripts/ShipScript.hpp"
#include "Utility/VectorConverter.hpp"
#include "box2d/box2d.h"

#include <cassert>
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
	: m_worldView(mainWindow.getDefaultView())
	, m_mainWindow(mainWindow)
	, m_box2dDebugDraw(mainWindow)
{
	m_registry.ctx().emplace<GameStateComponent>();
	m_worldView.setCenter(0,0);
	m_worldView.setSize(b2Vec2ToSfVec(toMeters(m_worldView.getSize())));

	// Initializes box2d debug draw interface.
	m_box2dDebugDraw.SetFlags(
		Box2dDebugDraw::e_shapeBit
		| Box2dDebugDraw::e_jointBit
		| Box2dDebugDraw::e_aabbBit
		| Box2dDebugDraw::e_pairBit
		| Box2dDebugDraw::e_centerOfMassBit
	);

	// Attach event-driven deallocators.
	//
	// TODO(zndf): Consider what happens to b2Body runtime instances stored in
	// RigidbodyComponent when stopPhysics is called and its implication to the
	// deallocators.
	m_registry.on_destroy<NativeScriptComponent>().connect<&Scene::deallocateNscInstance>();
	m_registry.on_destroy<RigidbodyComponent>().connect<&Scene::deallocateB2BodyInstance>();
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

// Bootstraps entity rigidbody runtime instances.
//
// NOTE: m_physicsWorld must already be freed before calling this method,
//       failure to do so would leak the memory.
// NOTE: As of now, both startPhysics and stopPhysics should only be called once
//       in a Scene's whole lifetime. Since in _not_ doing so would risk having
//       dangling references to freed b2Body runtime instances inside the
//       Scene's registry (Since calling stopPhysics doesn't remove all
//       RigidbodyComponent, and ShapeColliderComponents from preexisting
//       entities).
void Scene::startPhysics()
{
	m_physicsWorld = new b2World({0.0f, -9.8f});

	m_physicsWorld->SetDebugDraw(&m_box2dDebugDraw);

	auto view = m_registry.view<RigidbodyComponent>();
	for (auto e : view)
	{
		// Initializes runtime b2body with configuration contained in RigidbodyComponent
		Entity entity = { e, m_registry };
		auto& transform = entity.getComponent<TransformComponent>();
		auto& rb = entity.getComponent<RigidbodyComponent>();

		b2BodyDef bodyDef;
		bodyDef.type = rb.type;
		bodyDef.position.Set(transform.getPosition().x, transform.getPosition().y);
		bodyDef.angle = thor::toRadian(transform.getRotation());

		b2Body* body = m_physicsWorld->CreateBody(&bodyDef);
		body->SetFixedRotation(rb.fixedRotation);
		rb.runtimeBody = body;

		// Initializes the runtime b2body fixtures with configuration contained in
		// ShapeColliderComponents (ex. BoxColliderComponent)
		if (entity.hasComponent<BoxColliderComponent>())
		{
			auto& bc = entity.getComponent<BoxColliderComponent>();

			b2PolygonShape boxShape;
			boxShape.SetAsBox(bc.size.x * transform.getScale().x, bc.size.y * transform.getScale().y);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &boxShape;
			fixtureDef.density = bc.density;
			fixtureDef.friction = bc.friction;
			fixtureDef.restitution = bc.restitution;
			fixtureDef.restitutionThreshold = bc.restitutionThreshold;

			b2Fixture* fixture =  body->CreateFixture(&fixtureDef);
			bc.runtimeFixture = fixture;
		}

		if (entity.hasComponent<CircleColliderComponent>())
		{
			auto& cc = entity.getComponent<CircleColliderComponent>();

			b2CircleShape circleShape;
			circleShape.m_p.Set(cc.offset.x, cc.offset.y);
			circleShape.m_radius = transform.getScale().x * cc.radius;

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &circleShape;
			fixtureDef.density = cc.density;
			fixtureDef.friction = cc.friction;
			fixtureDef.restitution = cc.restitution;
			fixtureDef.restitutionThreshold = cc.restitutionThreshold;

			b2Fixture* fixture = body->CreateFixture(&fixtureDef);
			cc.runtimeFixture = fixture;
		}
	}
}

// Deletes physics world.
// NOTE: deleting nullptr is a well defined behavior, the operator will just do
//       nothing.
void Scene::stopPhysics()
{
	delete m_physicsWorld;
	m_physicsWorld = nullptr;
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

// Syncs entity's rigidbody transform with its independent TransformComponent
// since the render system (as in ECS) relies in TransformComponent and not
// rigidbody's tranform.
//
// also steps up the physics world.
void Scene::fixedUpdatePhysics(float deltaTime)
{
	m_physicsWorld->Step(deltaTime, m_velocityIterations, m_positionIterations);

	auto view = m_registry.view<RigidbodyComponent>();
	for (auto e : view)
	{
		Entity entity = { e, m_registry };
		auto& tc = entity.getComponent<TransformComponent>();
		auto& rb = entity.getComponent<RigidbodyComponent>();

		b2Body* body = rb.runtimeBody;
		const auto& position = body->GetPosition();
		tc.setPosition(position.x, position.y);
		tc.setRotation(thor::toDegree(body->GetAngle()));
	}
}

// Draws FPS, Frame Time indicator, and Scene Inspectors
// TODO(zndf): should've been moved to its own class
void Scene::drawEditorInterface()
{
	const auto& deltaTime = Time::getDeltaTime();
	ImGui::Begin("Engine Loop Stats");
	ImGui::LabelText("FPS", "%f", 1 / deltaTime.asSeconds());
	ImGui::LabelText("Frame Time", "%f", deltaTime.asSeconds());
	ImGui::End();

	displayComponentInspector(m_registry);
	displayEntityList(m_registry);
}

// Draws scene with m_worldView view
void Scene::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::View lastView = target.getView();

	target.setView(m_worldView);

	if (m_physicsWorld) m_physicsWorld->DebugDraw();

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

// An entt event-driven function that deallocates b2Body instances automatically
// when the RigidBodyComponent it is contained in is destroyed.
void Scene::deallocateB2BodyInstance(entt::registry& registry, entt::entity entity)
{
	auto& rb = registry.get<RigidbodyComponent>(entity);

	if (rb.runtimeBody)
	{
		b2World* world = rb.runtimeBody->GetWorld();
		world->DestroyBody(rb.runtimeBody);
	}
}

Scene::~Scene()
{
	m_registry.clear();
}

}
