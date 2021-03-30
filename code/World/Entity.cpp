#include "World/Entity.hpp"

#include "World/Components/Interfaces/InputComponent.hpp"
#include "World/Components/Interfaces/GraphicsComponent.hpp"
#include "World/Components/Interfaces/PhysicsComponent.hpp"

namespace astro
{

Entity::Entity(
	World& world,
	Type type,
	InputComponent& input,
	PhysicsComponent& physics,
	GraphicsComponent& graphics
)
: m_type { type }
, m_input { &input }
, m_physics { &physics }
, m_graphics { &graphics }
, m_world { world }
{

}

void Entity::handleEvent(sf::Event event)
{
	m_input->handleEvent(*this, event);
}

void Entity::update(float deltaTime)
{
	m_physics->update(*this, m_world);
}

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_graphics, states);
}

};
