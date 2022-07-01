#pragma once

#include "World/Entity.hpp"

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

	virtual void onUpdate(float deltaTime) {}
	virtual void onFixedUpdate(float deltaTime) {}

	virtual void onEvent(sf::Event event) {}

private:
	Entity m_entity;
	friend class World;
};

}
