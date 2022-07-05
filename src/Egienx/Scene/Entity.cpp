#include "Scene/Entity.hpp"

namespace enx
{

Entity::Entity(entt::entity handle, entt::registry& registry)
	: m_entityHandle(handle), m_registry(&registry)
{

}

} // namespace enx
