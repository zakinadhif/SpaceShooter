#pragma once

#include <SFML/Window/Event.hpp>

namespace astro
{

class Entity;
class EntityEvent;

class InputComponent
{
public:
	virtual void handleEvent(Entity& entity, sf::Event event) = 0;
	virtual void recieve(EntityEvent& event) {};

	virtual ~InputComponent() = default;
};

}
