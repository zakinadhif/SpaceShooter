#include "World/Systems.hpp"
#include "Utility/VectorConverter.hpp"
#include "World/Components/MeshComponent.hpp"
#include "World/Components/OwningMeshComponent.hpp"
#include "World/Components/RigidBodyComponent.hpp"

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
		const b2Body* const body = registry.get<RigidBodyComponent>(entity).body;

		sf::Transform transform;
		transform.translate(b2Vec2ToSfVec(body->GetPosition()));
		transform.rotate(thor::toDegree(body->GetAngle()));
		
		if (registry.has<MeshComponent>(entity))
		{
			const MeshComponent mesh = registry.get<MeshComponent>(entity);
			target.draw(mesh.vertices, mesh.size, mesh.type, sf::RenderStates(transform));
		}

		if (registry.has<OwningMeshComponent>(entity))
		{
			const OwningMeshComponent& mesh = registry.get<OwningMeshComponent>(entity);
			target.draw(mesh.vertices.data(), mesh.vertices.size(), mesh.type, sf::RenderStates(transform));
		}
	}
}

} // namespace astro
