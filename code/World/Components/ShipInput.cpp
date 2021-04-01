#include "World/Components/ShipInput.hpp"

#include "Utility/Keyboard.hpp"
#include "World/Entity.hpp"
#include "World/EntityEvent.hpp"

namespace astro
{

void ShipInput::handleEvent(Entity &entity, sf::Event event)
{
	using EntityEventType = EntityEvent::Type;
	using ShipDirection = EntityEvent::Direction;

	if (Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		EntityEvent event;
		event.type = EntityEventType::Direction;
		event.direction = ShipDirection::UP;
		entity.sendEvent(event);
	}
	
	if (Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		EntityEvent event;
		event.type = EntityEventType::Direction;
		event.direction = ShipDirection::DOWN;
		entity.sendEvent(event);
	}

	if (Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		EntityEvent event;
		event.type = EntityEventType::Direction;
		event.direction = ShipDirection::LEFT;
		entity.sendEvent(event);
	}

	if (Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		EntityEvent event;
		event.type = EntityEventType::Direction;
		event.direction = ShipDirection::RIGHT;
		entity.sendEvent(event);
	}
}

}
