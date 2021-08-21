#include "World/World.hpp"
#include "World/Systems.hpp"
#include "Utility/VectorConverter.hpp"
#include "Utility/Random.hpp"
#include "World/Components/MeshComponent.hpp"
#include "World/Components/AsteroidComponent.hpp"
#include "World/Components/OwningMeshComponent.hpp"
#include "World/Components/RigidBodyComponent.hpp"
#include "World/Components/TransformComponent.hpp"
#include "World/Components/BulletComponent.hpp"
#include "World/Builders/AsteroidBuilder.hpp"
#include "World/Entity.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <Thor/Math/Trigonometry.hpp>
#include <box2d/box2d.h>
#include <entt/entt.hpp>

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
	float x = zfge::Random::getFloat(spawnArea.left, spawnArea.left + spawnArea.width);
	float y = zfge::Random::getFloat(spawnArea.top, spawnArea.top + spawnArea.height);

	enum Sides {Left, Right, Top, Bottom};

	Sides side = (Sides) zfge::Random::getInt(0, 3);

	switch (side)
	{
		case Left:
			x = spawnArea.left;
			break;
		case Right:
			x = spawnArea.left + spawnArea.width;
			break;
		case Top:
			y = spawnArea.top;
			break;
		case Bottom:
			y = spawnArea.top + spawnArea.height;
			break;
	}

	builder.setPosition({x, y});
	builder.setAngularVelocity(thor::toRadian(zfge::Random::getFloat(-30, 30)));
}

} // namespace astro
