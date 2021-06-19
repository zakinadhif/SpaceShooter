#pragma once

#include "World/EntityType.hpp"

#include <string>

namespace astro
{

struct IdentifierComponent
{
	EntityType type;
	std::string name;
};

} // namespace astro
