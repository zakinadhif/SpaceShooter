#pragma once

#include "Scene/Components/Components.hpp"
#include "entt/signal/sigh.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <entt/entt.hpp>

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

	void handleEvent(const sf::Event& handleEvent);

	void updateScripts(float deltaTime);
	void fixedUpdateScripts(float deltaTime);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	~Scene();

	friend class SceneInspectorPanel;
	friend class EntityInspectorPanel;

private:
	const int m_velocityIterations = 6;
	const int m_positionIterations = 2;

	sf::View m_worldView;

	sf::RenderTarget& m_mainWindow;

	entt::registry m_registry;
	sf::Font m_munroFont;

	uint64_t m_lastEntityId = 0;

private:
	entt::connection m_nscDeallocatorConnection {};

	static void deallocateNscInstance(entt::registry& registry, entt::entity entity);
};

}
