#pragma once

#include <SFML/Window/Event.hpp>

namespace astro
{

class Entity;

class InputComponent
{
public:
	virtual void handleEvent(Entity& entity, sf::Event event) = 0;
};

}
