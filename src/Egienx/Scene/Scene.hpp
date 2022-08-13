#pragma once

#include "Scene/Components/Components.hpp"
#include "Utility/Box2dDebugDraw.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <entt/entt.hpp>
#include <box2d/b2_world.h>

namespace enx
{

class Entity;

class Scene : public sf::Drawable
{
public:
	Scene(sf::RenderTarget& mainWindow);

	Entity createEntity(const std::string& name = "");
	Entity createEntityWithID(IDComponent::IDType id, const std::string& name);

	static std::unique_ptr<Scene> clone(Scene& scene);

	void startPhysics();
	void stopPhysics();

	void handleEvent(const sf::Event& handleEvent);

	void updateScripts(float deltaTime);
	void fixedUpdateScripts(float deltaTime);
	void fixedUpdatePhysics(float deltaTime);

	void drawEditorInterface();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	~Scene();

	friend class EntityInspectorPanel;

private:
	const int m_velocityIterations = 6;
	const int m_positionIterations = 2;

	b2World* m_physicsWorld {};
	sf::View m_worldView;

	sf::RenderTarget& m_mainWindow;

	Box2dDebugDraw m_box2dDebugDraw;

	entt::registry m_registry;
	sf::Font m_munroFont;

	uint64_t m_lastEntityId = 0;

	bool m_isPhysicsStarted = false;

private:
	static void deallocateNscInstance(entt::registry& registry, entt::entity entity);
	static void deallocateB2BodyInstance(entt::registry& registry, entt::entity entity);
};

}
