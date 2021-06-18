#pragma once

#include "World/ScriptableEntity.hpp"

#include <functional>

namespace astro
{

struct NativeScriptComponent
{
	ScriptableEntity* instance;

	std::function<ScriptableEntity*()> instantiateScript;
	void (*destroyScript)(NativeScriptComponent*);

	template <typename T, typename... Args>
	void bind(Args&&... args)
	{
		instantiateScript = [&]() { return static_cast<ScriptableEntity*>(new T(std::forward<Args>(args)...)); };
		destroyScript = [](NativeScriptComponent* nsc) { delete nsc->instance; nsc->instance = nullptr; };
	}
};

} // namespace astro
