#include "World/Systems.hpp"
#include "Utility/VectorConverter.hpp"
#include "World/Components/MeshComponent.hpp"
#include "World/Components/AsteroidComponent.hpp"
#include "World/Components/OwningMeshComponent.hpp"
#include "World/Components/RigidBodyComponent.hpp"
#include "World/Components/BulletComponent.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <Thor/Math/Trigonometry.hpp>
#include <box2d/box2d.h>
#include <entt/entt.hpp>

#include "World/Components/TransformComponent.hpp"

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

} // namespace astro
