#pragma once

#include "World/Entity.hpp"

#include <box2d/b2_world.h>
#include <Thor/Resources.hpp>

namespace astro
{

class World : public sf::Drawable
{
public:
	World();

	void update(float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	b2World m_physicsWorld;

	std::vector<Entity> m_entities;

	thor::ResourceHolder<sf::Texture, std::string> m_textures;
};

}
