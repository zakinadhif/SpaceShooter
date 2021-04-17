#pragma once

#include "World/Components/Interfaces/InputComponent.hpp"

#include <SFML/Window/Keyboard.hpp>

namespace astro
{

class Entity;
class CoordinateSpaceMapper;

class ShipInput final : public InputComponent
{
public:
	ShipInput(const CoordinateSpaceMapper& worldSpaceMapper);

	void handleEvent(const sf::Event& event, Entity& entity) override;

private:
	void handleKeyPress(sf::Keyboard::Key key, Entity& entity);
	void handleKeyRelease(sf::Keyboard::Key key, Entity& entity);
	void handleMouseMove(sf::Vector2i position, Entity& entity);

	const CoordinateSpaceMapper& m_worldSpaceMapper;
};

}

