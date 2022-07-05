#pragma once

#include "Scene/ScriptableEntity.hpp"

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <vector>
#include <bits/c++config.h>
#include <functional>
#include <string>

namespace sf
{
class Vertex;
}

namespace enx
{

struct IdentifierComponent
{
	std::string name;
};

// struct IDComponent
// {
// 	UUID id;
// };

struct GameStateComponent
{
	float score = 0.f;
	int health = 3.f;

	int screenWidth = 0.f;
	int screenHeight = 0.f;

	uint32_t selectedEntity = 0;
};

struct TagComponent
{
	std::string tag;
};

using TransformComponent = sf::Transformable;

struct MeshComponent
{
	sf::Vertex* vertices;
	std::size_t size;
	sf::PrimitiveType type;
};

struct OwningMeshComponent
{
	std::vector<sf::Vertex> vertices;
	sf::PrimitiveType type;
};

class ScriptableEntity;

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

} // namespace enx
