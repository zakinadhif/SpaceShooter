#include "World/Components/RigidBodyComponent.hpp"
#include "World/UnitScaling.hpp"

#include "Utility/VectorConverter.hpp"

#include <box2d/box2d.h>

namespace astro
{
	void RigidBodyComponent::setTransform(const sf::Vector2f& position, float angle)
	{
		body->SetTransform(toMeters(position), angle);
	}

	b2Transform RigidBodyComponent::getTransform() const
	{
		const auto& transform = body->GetTransform();

		return {sfVec2ToB2Vec(toPixels(transform.p)), transform.q};
	}

	sf::Vector2f RigidBodyComponent::getPosition() const
	{
		return toPixels(body->GetPosition());
	}

	sf::Vector2f RigidBodyComponent::getWorldCenter() const
	{
		return toPixels(body->GetWorldCenter());
	}

	sf::Vector2f RigidBodyComponent::getLocalCenter() const
	{
		return toPixels(body->GetLocalCenter());
	}

	void RigidBodyComponent::setLinearVelocity(const sf::Vector2f& velocity)
	{
		body->SetLinearVelocity(toMeters(velocity));
	}

	sf::Vector2f RigidBodyComponent::getLinearVelocity() const
	{
		return toPixels(body->GetLinearVelocity());
	}

	void RigidBodyComponent::applyForce(const sf::Vector2f& force, const sf::Vector2f& point, bool wake)
	{
		body->ApplyForce(toMeters(force), toMeters(point), wake);
	}

	void RigidBodyComponent::applyForceToCenter(const sf::Vector2f& force, bool wake)
	{
		body->ApplyForceToCenter(toMeters(force), wake);
	}

	void RigidBodyComponent::applyLinearImpulse(const sf::Vector2f& impulse, const sf::Vector2f& point, bool wake)
	{
		body->ApplyLinearImpulse(toMeters(impulse), toMeters(point), wake);
	}

	void RigidBodyComponent::applyLinearImpulseToCenter(const sf::Vector2f& impulse, bool wake)
	{
		body->ApplyLinearImpulseToCenter(toMeters(impulse), wake);
	}

	sf::Vector2f RigidBodyComponent::getWorldPoint(const sf::Vector2f& localPoint) const
	{
		return toPixels(body->GetWorldPoint(toMeters(localPoint)));
	}

	sf::Vector2f RigidBodyComponent::getWorldVector(const sf::Vector2f& localVector) const
	{
		return toPixels(body->GetWorldVector(toMeters(localVector)));
	}

	sf::Vector2f RigidBodyComponent::getLocalPoint(const sf::Vector2f& worldPoint) const
	{
		return toPixels(body->GetLocalPoint(toMeters(worldPoint)));
	}

	sf::Vector2f RigidBodyComponent::getLocalVector(const sf::Vector2f& worldVector) const
	{
		return toPixels(body->GetLocalVector(toMeters(worldVector)));
	}

	sf::Vector2f RigidBodyComponent::getLinearVelocityFromWorldPoint(const sf::Vector2f& worldPoint) const
	{
		return toPixels(body->GetLinearVelocityFromWorldPoint(toMeters(worldPoint)));
	}

	sf::Vector2f RigidBodyComponent::getLinearVelocityFromLocalPoint(const sf::Vector2f& localPoint) const
	{
		return toPixels(body->GetLinearVelocityFromLocalPoint(toMeters(localPoint)));
	}
}
