#pragma once

#include "Scene/Entity.hpp"

#include <SFML/Window/Event.hpp>

namespace enx
{

class ScriptableEntity
{
public:
	virtual ~ScriptableEntity() {};

	template<typename T>
	T& getComponent()
	{
		return m_entity.getComponent<T>();
	}

protected:
	virtual void onCreate() {}
	virtual void onDestroy() {}

	virtual void onUpdate([[maybe_unused]] float deltaTime) {}
	virtual void onFixedUpdate([[maybe_unused]] float deltaTime) {}

	virtual void onEvent([[maybe_unused]] sf::Event event) {}

private:
	Entity m_entity;
	friend class Scene;
};

}
