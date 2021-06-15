#pragma once

#include "Utility/Box2dDebugDraw.hpp"
#include "World/CoordinateSpaceMapper.hpp"

#include <Thor/Shapes/ConcaveShape.hpp>

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <entt/entity/registry.hpp>
#include <box2d/b2_world.h>

namespace astro
{

class Entity;

class World : public sf::Drawable
{
public:
	World(sf::RenderTarget& mainWindow);

	const CoordinateSpaceMapper& getWorldSpaceMapper() const;

	Entity createEntity();

	void handleEvent(const sf::Event& handleEvent);
	void update(float deltaTime);
	void fixedUpdate(float deltaTime);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	const int m_velocityIterations = 6;
	const int m_positionIterations = 2;

	mutable b2World m_physicsWorld;
	sf::View m_worldView;

	sf::RenderTarget& m_mainWindow;

	CoordinateSpaceMapper m_worldSpaceMapper;
	Box2dDebugDraw m_box2dDebugDraw;

	entt::registry m_registry;
};

}
