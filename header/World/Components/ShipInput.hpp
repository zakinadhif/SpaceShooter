#pragma once

#include "World/Components/Interfaces/InputComponent.hpp"

#include <SFML/Window/Keyboard.hpp>

namespace astro
{

class Entity;

class ShipInput final : public InputComponent
{
public:
	void handleEvent(const sf::Event& event, Entity& entity) override;

private:
	void handleKeyPress(sf::Keyboard::Key key, Entity& entity);
	void handleKeyRelease(sf::Keyboard::Key key, Entity& entity);
};

}

