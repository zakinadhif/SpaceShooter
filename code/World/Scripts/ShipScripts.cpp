#include "World/Scripts/ShipScript.hpp"

#include "spdlog/spdlog.h"

namespace astro
{

ShipScript::ShipScript()
{

}

void ShipScript::onCreate()
{
	spdlog::info("A shipscript have just been instantiated.");
}

void ShipScript::onDestroy()
{
	spdlog::info("A shipscript instance have just been destroyed.");
}

void ShipScript::onUpdate(float deltaTime)
{

}

} // namespace astro
