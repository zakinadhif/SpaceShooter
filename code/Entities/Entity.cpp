#include "Entities/Entity.hpp"

#include "Entities/Components/InputComponent.hpp"
#include "Entities/Components/GraphicsComponent.hpp"
#include "Entities/Components/PhysicsComponent.hpp"

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
