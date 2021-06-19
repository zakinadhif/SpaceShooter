#include "World/Entity.hpp"

namespace astro
{

Entity::Entity(entt::entity handle, entt::registry& registry)
	: m_entityHandle(handle), m_registry(&registry)
{

}

} // namespace astro
