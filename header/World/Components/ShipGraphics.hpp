#pragma once

#include "World/Components/Interfaces/GraphicsComponent.hpp"

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include <array>

class b2Body;

namespace astro
{

class PhysicsComponent;

class ShipGraphics final : public GraphicsComponent
{
public:
	ShipGraphics(PhysicsComponent* physics);

	void update(Entity& entity) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	std::array<sf::Vertex, 3> m_model;
	sf::Transformable m_transform;

	const b2Body* m_physicsBody;
};

}
