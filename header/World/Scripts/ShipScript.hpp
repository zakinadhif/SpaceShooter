#pragma once

#include "World/ScriptableEntity.hpp"

namespace astro
{

class ShipScript final : public ScriptableEntity
{
public:
	ShipScript();

	void onCreate() override;
	void onDestroy() override;
	void onUpdate(float deltaTime) override;
};

} // namespace astro
