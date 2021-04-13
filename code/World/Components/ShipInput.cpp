#include "World/Components/ShipInput.hpp"

#include "Utility/Keyboard.hpp"
#include "World/Entity.hpp"
#include "World/EntityEvent.hpp"

namespace
{
	void instructMove(astro::Entity& entity, astro::EntityEvent::Direction direction)
	{
		astro::EntityEvent event
		{
			.type = astro::EntityEvent::Type::Move,
			.direction = direction
		};
		entity.sendEvent(event);
	}

	void instructStop(astro::Entity& entity, astro::EntityEvent::Direction direction)
	{
		astro::EntityEvent event
		{
			.type = astro::EntityEvent::Type::StopMove,
			.direction = direction
		};
		entity.sendEvent(event);
	}
}

namespace astro
{

void ShipInput::handleEvent(const sf::Event& event, Entity& entity)
{
	switch (event.type)
	{
		case sf::Event::KeyPressed:
			handleKeyPress(event.key.code, entity);
			break;
		case sf::Event::KeyReleased:
			handleKeyRelease(event.key.code, entity);
			break;
		default:
			break;
	}
}

void ShipInput::handleKeyPress(sf::Keyboard::Key key, Entity& entity)
{
	using Direction = EntityEvent::Direction;
	using Key = sf::Keyboard::Key;

	switch (key)
	{
		case Key::Up:
			instructMove(entity, Direction::UP);
			break;
		case Key::Down:
			instructMove(entity, Direction::DOWN);
			break;
		case Key::Left:
			instructMove(entity, Direction::LEFT);
			break;
		case Key::Right:
			instructMove(entity, Direction::RIGHT);
			break;
		default: break;
	}
}

void ShipInput::handleKeyRelease(sf::Keyboard::Key key, Entity& entity)
{
	using Direction = EntityEvent::Direction;
	using Key = sf::Keyboard::Key;

	switch (key)
	{
		case Key::Up:
			instructStop(entity, Direction::UP);
			break;
		case Key::Down:
			instructStop(entity, Direction::DOWN);
			break;
		case Key::Left:
			instructStop(entity, Direction::LEFT);
			break;
		case Key::Right:
			instructStop(entity, Direction::RIGHT);
			break;
		default: break;
	}
}

}
