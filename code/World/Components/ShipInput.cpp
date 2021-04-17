#include "World/Components/ShipInput.hpp"

#include "Utility/Keyboard.hpp"
#include "World/CoordinateSpaceMapper.hpp"
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

	void instructLookAt(astro::Entity& entity, sf::Vector2f point)
	{
		astro::EntityEvent event
		{
			.type = astro::EntityEvent::Type::PointTo,
			.point = point
		};
		entity.sendEvent(event);
	}
}

namespace astro
{

ShipInput::ShipInput(const CoordinateSpaceMapper& worldSpaceMapper)
	: m_worldSpaceMapper(worldSpaceMapper)
{

}

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
		case sf::Event::MouseMoved:
			handleMouseMove({event.mouseMove.x, event.mouseMove.y}, entity);
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

void ShipInput::handleMouseMove(sf::Vector2i position, Entity& entity)
{
	instructLookAt(entity, m_worldSpaceMapper.mapToViewSpace(position));
}

}
