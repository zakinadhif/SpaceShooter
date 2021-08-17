#include "World/EntityFactory.hpp"

#include "Utility/VectorConverter.hpp"
#include "World/Components/Components.hpp"
#include "World/Components/RigidBodyComponent.hpp"
#include "World/PhysicsBodyFactory.hpp"
#include "World/UnitScaling.hpp"

#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Color.hpp>
#include <box2d/box2d.h>
#include <entt/entt.hpp>

#include <array>

namespace astro
{

entt::entity spawnShip(World& world, const sf::Vector2f& position, b2World* physicsWorld)
{
	static std::array<sf::Vertex, 3> shipMesh
	{
		{
			{{toPixels(0.f, -0.5f)}, sf::Color::White},
			{{toPixels(-0.5f, 0.5f)}, sf::Color::White},
			{{toPixels(0.5f, 0.5f)}, sf::Color::White}
		}
	};

	Entity entity = world.createEntity();

	entity.addComponent<MeshComponent>(shipMesh.data(), shipMesh.size(), sf::Triangles);
	auto& rgc = entity.addComponent<RigidBodyComponent>(createShipBody(physicsWorld, sfVec2ToB2Vec(position)));

	rgc.body->GetUserData().pointer = (uintptr_t) new Entity(entity);

	return entity;
}

} // namespace astro
