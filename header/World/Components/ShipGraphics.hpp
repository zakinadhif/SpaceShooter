#pragma once

#include "World/Components/Interfaces/GraphicsComponent.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <Thor/Resources/ResourceHolder.hpp>

class b2Body;

namespace astro
{

class PhysicsComponent;

class ShipGraphics final : public GraphicsComponent
{
public:
	ShipGraphics(
			thor::ResourceHolder<sf::Texture, std::string>& textureHolder,
			PhysicsComponent* physics);

	void update(Entity& entity) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	sf::Sprite m_sprite;
	const b2Body* m_physicsBody;
};

}
