#include "World/Entity.hpp"

#include "World/Components/Interfaces/InputComponent.hpp"
#include "World/Components/Interfaces/GraphicsComponent.hpp"
#include "World/Components/Interfaces/PhysicsComponent.hpp"

#include "World/EntityEvent.hpp"

namespace astro
{

Entity::Entity(
	World& world,
	Type type,
	InputComponent* input,
	PhysicsComponent* physics,
	GraphicsComponent* graphics
)
: m_type { type }
, m_input { input }
, m_physics { physics }
, m_graphics { graphics }
, m_world { world }
{

}

Entity::Entity(Entity&& other)
: m_type { other.m_type }
, m_input { other.m_input }
, m_physics { other.m_physics }
, m_graphics { other.m_graphics }
, m_world { other.m_world }
{
	other.m_type = Type::Nothing;
	other.m_input = nullptr;
	other.m_physics = nullptr;
	other.m_graphics = nullptr;
}

void Entity::update(float deltaTime)
{
	m_input->update(*this);
	m_graphics->update(*this);
}

void Entity::fixedUpdate(float deltaTime)
{
	m_physics->fixedUpdate(*this, deltaTime);
}

void Entity::sendEvent(EntityEvent event)
{
	m_input->recieve(event);
	m_physics->recieve(event);
	m_graphics->recieve(event);
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_graphics, states);
}

World& Entity::getWorld()
{
	return m_world;
}

const World& Entity::getWorld() const
{
	return m_world;
}

Entity::~Entity()
{
	delete m_input;
	delete m_physics;
	delete m_graphics;
}

};
