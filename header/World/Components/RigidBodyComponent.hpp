#pragma once

#include <SFML/System/Vector2.hpp>

class b2Body;
class b2Transform;

namespace enx
{

struct RigidBodyComponent
{
	b2Body* body;

	void setTransform(const sf::Vector2f& position, float angle);
	b2Transform getTransform() const;

	sf::Vector2f getPosition() const;

	sf::Vector2f getWorldCenter() const;
	sf::Vector2f getLocalCenter() const;

	void setLinearVelocity(const sf::Vector2f& velocity);
	sf::Vector2f getLinearVelocity() const;

	void applyForce(const sf::Vector2f& force, const sf::Vector2f& point, bool wake);
	void applyForceToCenter(const sf::Vector2f& force, bool wake);

	void applyLinearImpulse(const sf::Vector2f& impulse, const sf::Vector2f& point, bool wake);
	void applyLinearImpulseToCenter(const sf::Vector2f& impulse, bool wake);

	sf::Vector2f getWorldPoint(const sf::Vector2f& localPoint) const;
	sf::Vector2f getWorldVector(const sf::Vector2f& localVector) const;

	sf::Vector2f getLocalPoint(const sf::Vector2f& worldPoint) const;
	sf::Vector2f getLocalVector(const sf::Vector2f& worldVector) const;

	sf::Vector2f getLinearVelocityFromWorldPoint(const sf::Vector2f& worldPoint) const;
	sf::Vector2f getLinearVelocityFromLocalPoint(const sf::Vector2f& localPoint) const;
};

} // namespace enx
