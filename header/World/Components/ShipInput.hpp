#pragma once

#include "World/Components/Interfaces/InputComponent.hpp"

#include <SFML/Window/Keyboard.hpp>

namespace astro
{

class Entity;

class ShipInput final : public InputComponent
{
public:
	void update(Entity& entity) override;
};

}

