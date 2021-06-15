#pragma once

#include "World/ScriptableEntity.hpp"

namespace astro
{

struct NativeScriptComponent
{
	ScriptableEntity* instance;

	ScriptableEntity* (*instantiateScript)();
	void (*destroyScript)(NativeScriptComponent*);

	template <typename T>
	void bind()
	{
		instantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
		destroyScript = [](NativeScriptComponent* nsc) { delete nsc->instance; nsc->instance = nullptr; };
	}
};

} // namespace astro
