#pragma once

#include <SFML/Window/Event.hpp>

namespace astro
{

class Entity;
class EntityEvent;

class InputComponent
{
public:
	virtual void update(Entity& entity) = 0;
	virtual void recieve(EntityEvent& event) {};

	virtual ~InputComponent() = default;
};

}
