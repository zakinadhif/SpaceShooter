#include "World/ContactListener.hpp"

#include "World/Components/AsteroidComponent.hpp"
#include "World/Components/BulletComponent.hpp"
#include "World/Entity.hpp"

#include <box2d/b2_contact.h>
#include <spdlog/spdlog.h>

namespace astro
{

ContactListener::ContactListener()
{

}

void ContactListener::BeginContact(b2Contact *contact)
{
	spdlog::trace("Fixture End Contact: {} with {}", (void*) contact->GetFixtureA()->GetBody(), (void*) contact->GetFixtureB()->GetBody());

	Entity entityA = *((Entity*) contact->GetFixtureA()->GetBody()->GetUserData().pointer);
	Entity entityB = *((Entity*) contact->GetFixtureB()->GetBody()->GetUserData().pointer);

	if ((entityA.hasComponent<BulletComponent>() && entityB.hasComponent<AsteroidComponent>())
		|| (entityA.hasComponent<AsteroidComponent>() && entityB.hasComponent<BulletComponent>()))
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
}

void ContactListener::EndContact(b2Contact *contact)
{
	spdlog::info("Fixture Begin Contact: {} with {}", (void*) contact->GetFixtureA()->GetBody(), (void*) contact->GetFixtureB()->GetBody());
}

}
