#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

namespace astro
{

class InputComponent;
class PhysicsComponent;
class GraphicsComponent;

class World;

struct EntityEvent;

class Entity : public sf::Drawable
{
public:
	enum class Type
	{
		Nothing,
		Ship,
		Asteroid
	};

public:

	Entity(
		World& world,
		Type type,
		InputComponent* input,
		PhysicsComponent* physics,
		GraphicsComponent* graphics
	);

	Entity(Entity&& entity);
	Entity(Entity& copy) = delete;

	void handleEvent(sf::Event event);
	void update(float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void sendEvent(EntityEvent event);

	Type getType() const;

	World& getWorld();
	const World& getWorld() const;

	~Entity();

private:
	Type m_type { Type::Nothing };

	InputComponent* m_input;
	PhysicsComponent* m_physics;
	GraphicsComponent* m_graphics;

	World& m_world;
};

}
