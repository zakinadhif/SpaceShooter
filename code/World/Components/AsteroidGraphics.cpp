#include "World/Components/AsteroidGraphics.hpp"

namespace astro
{

AsteroidGraphics::AsteroidGraphics(const std::vector<sf::Vector2f>& outerVertices, PhysicsComponent& physics)
	: m_body(physics.getBody())
{
	m_shape.setPointCount(outerVertices.size());

	for (std::size_t x = 0; x < outerVertices.size(); ++x)
	{
		m_shape.setPoint(x, outerVertices[x]);
	}

	m_shape.setFillColor(sf::Color::White);
}

void AsteroidGraphics::update(Entity& entity)
{
}

void AsteroidGraphics::draw(sf::RenderTarget &target, sf::RenderStates states) const
{

}

} // namespace astro
