#pragma once

#include "World/Components/Interfaces/InputComponent.hpp"

namespace astro
{

class NullInput final : public InputComponent
{
public:
	void handleEvent(const sf::Event& event, Entity& entity) override {};
};

}
