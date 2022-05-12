#pragma once

#include <box2d/b2_world_callbacks.h>
#include <functional>

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
