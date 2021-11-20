#include "World/ContactListener.hpp"

#include "World/Components/AsteroidComponent.hpp"
#include "World/Components/BulletComponent.hpp"
#include "World/Components/IdentifierComponent.hpp"
#include "World/Entity.hpp"
#include "World/EntityType.hpp"

#include <box2d/b2_contact.h>
#include <spdlog/spdlog.h>

namespace astro
{

namespace
{
	bool asteroidAndBulletCollided(Entity a, Entity b)
	{
		return (a.hasComponent<BulletComponent>() && b.hasComponent<AsteroidComponent>())
			|| (a.hasComponent<AsteroidComponent>() && b.hasComponent<BulletComponent>());
	}

	bool shipAndAsteroidCollided(Entity a, Entity b)
	{
		if (a.hasComponent<IdentifierComponent>())
		{
			auto& ic = a.getComponent<IdentifierComponent>();
			return ic.type == EntityType::Ship && b.hasComponent<AsteroidComponent>();
		}
		else if (b.hasComponent<IdentifierComponent>())
		{
			auto& ic = b.getComponent<IdentifierComponent>();
			return ic.type == EntityType::Ship && a.hasComponent<AsteroidComponent>();
		}

		return false;
	}
}


ContactListener::ContactListener()
{

}

void ContactListener::BeginContact(b2Contact *contact)
{
	spdlog::trace("Fixture End Contact: {} with {}", (void*) contact->GetFixtureA()->GetBody(), (void*) contact->GetFixtureB()->GetBody());

	Entity entityA = *((Entity*) contact->GetFixtureA()->GetBody()->GetUserData().pointer);
	Entity entityB = *((Entity*) contact->GetFixtureB()->GetBody()->GetUserData().pointer);

	if (asteroidAndBulletCollided(entityA, entityB))
	{
		spdlog::trace("A bullet and an asteroid has just been collided.");
		if (entityA.hasComponent<AsteroidComponent>())
		{
			entityA.getComponent<AsteroidComponent>().shouldBeDestroyed = true;
			entityB.getComponent<BulletComponent>().shouldBeDestroyed = true;
		}
		if (entityB.hasComponent<AsteroidComponent>())
		{
			entityB.getComponent<AsteroidComponent>().shouldBeDestroyed = true;
			entityA.getComponent<BulletComponent>().shouldBeDestroyed = true;
		}
	}

	if (shipAndAsteroidCollided(entityA, entityB))
	{
		spdlog::info("The ship had just collided");
	}
}

void ContactListener::EndContact(b2Contact *contact)
{
	spdlog::info("Fixture Begin Contact: {} with {}", (void*) contact->GetFixtureA()->GetBody(), (void*) contact->GetFixtureB()->GetBody());
}

}
