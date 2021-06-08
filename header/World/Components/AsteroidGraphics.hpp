#pragma once

#include "World/Components/Interfaces/GraphicsComponent.hpp"
#include "World/Components/Interfaces/PhysicsComponent.hpp"

#include <Thor/Shapes.hpp>
#include <SFML/Graphics/Transformable.hpp>

namespace astro
{

class AsteroidGraphics final : public GraphicsComponent
{
public:
	AsteroidGraphics(const std::vector<sf::Vector2f>& outerVertices, PhysicsComponent& physics);

	void update(Entity& entity) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	const b2Body* m_body;

	thor::ConcaveShape m_shape;
	sf::Transformable m_transformable;
};

} // namespace astro
