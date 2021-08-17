#pragma once

#include <box2d/b2_world_callbacks.h>

namespace astro
{

class ContactListener : public b2ContactListener
{
public:
	ContactListener();

	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;
};

}
