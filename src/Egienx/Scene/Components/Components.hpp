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

struct IDComponent
{
	uint64_t id {};
};

struct GameStateComponent
{
	uint64_t lastEntityId = 0;
	uint64_t selectedEntity = 0;
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

struct RigidbodyComponent
{
	enum class BodyType { Static = 0, Dynamic, Kinematic };
	BodyType type = BodyType::Static;
	bool fixedRotation = false;

	// Storage for runtime
	void* runtimeBody = nullptr;
};

struct BoxColliderComponent
{
	sf::Vector2f offset = {};
	sf::Vector2f size = { 0.5f, 0.5f };

	float density = 1.0f;
	float friction = 0.5f;
	float restitution = 0.0f;
	float restitutionThreshold = 0.5f;

	void* runtimeFixture = nullptr;
};

struct CircleColliderComponent
{
	sf::Vector2f offset = {};
	float radius = 0.5f;

	float density = 1.0f;
	float friction = 0.5f;
	float restitution = 0.0f;
	float restitutionThreshold = 0.5f;

	void* runtimeFixture = nullptr;
};

template <typename... Components>
struct ComponentGroup
{
};

using AllComponents =
	ComponentGroup<IDComponent, TagComponent, MeshComponent,
		OwningMeshComponent, NativeScriptComponent, RigidbodyComponent,
		BoxColliderComponent, CircleColliderComponent>;

} // namespace enx
