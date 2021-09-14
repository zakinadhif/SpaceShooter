#include "World/World.hpp"
#include "World/Systems.hpp"
#include "Utility/VectorConverter.hpp"
#include "Utility/Random.hpp"
#include "World/Components/Components.hpp"
#include "World/Builders/AsteroidBuilder.hpp"
#include "World/Entity.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <Thor/Math/Trigonometry.hpp>
#include <box2d/box2d.h>
#include <entt/entt.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include <bitset>

namespace astro
{

void drawEntities(const entt::registry& registry, sf::RenderTarget& target)
{
	const auto view = registry.view<const RigidBodyComponent>();

	for (const auto& entity : view)
	{
		const auto& rbc = registry.get<RigidBodyComponent>(entity);
		const TransformComponent* tc = registry.try_get<TransformComponent>(entity);

		sf::Transform transform;
		transform.translate(rbc.getPosition());
		transform.rotate(thor::toDegree(rbc.body->GetAngle()));

		if (tc)
		{
			const auto& graphicsTransform = tc->getTransform();
			transform *= graphicsTransform;
		}

		if (registry.all_of<MeshComponent>(entity))
		{
			const MeshComponent mesh = registry.get<MeshComponent>(entity);
			target.draw(mesh.vertices, mesh.size, mesh.type, sf::RenderStates(transform));
		}

		if (registry.all_of<OwningMeshComponent>(entity))
		{
			const OwningMeshComponent& mesh = registry.get<OwningMeshComponent>(entity);
			target.draw(mesh.vertices.data(), mesh.vertices.size(), mesh.type, sf::RenderStates(transform));
		}
	}
}

void clearShotAsteroids(entt::registry &registry)
{
	auto view = registry.view<AsteroidComponent>();

	for (auto& entity : view)
	{
		const auto& ac = view.get<AsteroidComponent>(entity);

		if (ac.shouldBeDestroyed)
		{
			registry.destroy(entity);
		}
	}
}

void clearCollidedBullets(entt::registry& registry)
{
	auto view = registry.view<BulletComponent>();

	for (auto& entity : view)
	{
		const auto& bc = view.get<BulletComponent>(entity);

		if (bc.shouldBeDestroyed)
		{
			registry.destroy(entity);
		}
	}
}

void spawnAsteroidsRandomly(World& world, AsteroidBuilder& builder, sf::FloatRect spawnArea)
{
	const float speedMin = 50.f;
	const float speedMax = 150.f;
	const float rotationSpeedMin = -30.f;
	const float rotationSpeedMax = 30.f;

	float x = zfge::Random::getFloat(spawnArea.left, spawnArea.left + spawnArea.width);
	float y = zfge::Random::getFloat(spawnArea.top, spawnArea.top + spawnArea.height);

	float velocityMagnitude = zfge::Random::getFloat(speedMin, speedMax);
	float velocityAngle = 0.0f;

	float rotationSpeed = zfge::Random::getFloat(rotationSpeedMin, rotationSpeedMax);

	enum Sides {Left, Right, Top, Bottom};
	Sides side = (Sides) zfge::Random::getInt(0, 3);

	switch (side)
	{
		case Left:
			x = spawnArea.left;
			velocityAngle += zfge::Random::getFloat(-90.f, 90.f);
			// velocityAngle += 0.f;
			break;
		case Right:
			x = spawnArea.left + spawnArea.width;
			velocityAngle += zfge::Random::getFloat(90.f, 270.f);
			// velocityAngle += 180.f;
			break;
		case Top:
			y = spawnArea.top;
			velocityAngle += zfge::Random::getFloat(180.f, 360.f);
			// velocityAngle += 270.f;
			break;
		case Bottom:
			y = spawnArea.top + spawnArea.height;
			velocityAngle += zfge::Random::getFloat(0.f, 180.f);
			// velocityAngle += 90.f;
			break;
	}

	float _velocityAngle = thor::toRadian(velocityAngle);
	sf::Vector2f velocity {
		cos(_velocityAngle) * velocityMagnitude,
		-sin(_velocityAngle) * velocityMagnitude
	};

	float _rotationSpeed = thor::toRadian(rotationSpeed);

	builder.setPosition({x, y});
	builder.setAngularVelocity(_rotationSpeed);
	builder.setLinearVelocity(velocity);

	builder.spawn();
}

namespace
{

template<typename Component, std::size_t size>
void fillBitset(
	std::bitset<size>& bitset,
	std::size_t& index,
	entt::registry& registry,
	entt::entity entity
)
{
	bitset[index] = registry.all_of<Component>(entity);
	++index;
}

template<typename... Comps>
std::bitset<sizeof...(Comps)> getComponentBitset(entt::registry& registry, entt::entity entity)
{
	std::bitset<sizeof...(Comps)> bitset {};
	std::size_t index = 0;

	(fillBitset<Comps>(bitset, index, registry, entity), ...);

	return bitset;
}

}

void displayComponentInspector(entt::registry& registry)
{
	constexpr std::size_t componentCount = 7;

	// Name lookup table
	static const std::array<std::string, componentCount> componentNames {
		"IdentifierComponent",
		"TransformComponent",
		"MeshComponent",
		"OwningMeshComponent",
		"NativeScriptComponent",
		"BulletComponent",
		"AsteroidComponent"
	};

	// TODO(zndf): List entities. DONE.
	// TODO(zndf): Write name component. DONE. (subtituted with IdentifierComponent)
	// TODO(zndf): Write group component. DONE. (subtituted with IdentifierComponent)
	// TODO(zndf): Search a way to be able to edit and get component content.

	ImGui::Begin("Component Inspector");
	registry.each([&registry](entt::entity entity){
		std::bitset<componentCount> componentBitset = getComponentBitset<
			IdentifierComponent,
			TransformComponent,
			MeshComponent,
			OwningMeshComponent,
			NativeScriptComponent,
			BulletComponent,
			AsteroidComponent
		>(registry, entity);

		std::string strbuf = std::to_string((std::uint32_t) entity);
		ImGui::Text("Entity %s: ", strbuf.c_str());
		
		for (std::size_t i = 0; i < componentCount; ++i)
		{
			if (componentBitset[i])
			{
				ImGui::Text("%s", componentNames[i].c_str());
			}
		}
	});
	ImGui::End();
}

} // namespace astro
