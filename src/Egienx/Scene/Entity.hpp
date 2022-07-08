#pragma once

#include <entt/entt.hpp>

namespace enx
{

class Entity
{
public:
	Entity() = default;
	Entity(entt::entity handle, entt::registry& registry);

	Entity(const Entity& other) = default;
	Entity& operator=(const Entity& other) = default;

	template<typename T, typename... Args>
	T& addComponent(Args&&... args)
	{
		return m_registry->emplace<T>(m_entityHandle, std::forward<Args>(args)...);
	}

	template<typename T>
	T& getComponent()
	{
		return m_registry->get<T>(m_entityHandle);
	}

	template<typename T>
	bool hasComponent()
	{
		return m_registry->all_of<T>(m_entityHandle);
	}

	template<typename T>
	void removeComponent()
	{
		m_registry->remove<T>(m_entityHandle);
	}

	operator bool() const { return m_entityHandle != entt::null; }
	operator entt::entity() const { return m_entityHandle; }
	operator uint32_t() const { return (uint32_t)m_entityHandle; }

	bool operator==(const Entity& other) const
	{
		return (m_entityHandle == other.m_entityHandle) && (m_registry == other.m_registry);
	}

	bool operator!=(const Entity& other) const
	{
		return !(*this == other);
	}

private:
	entt::entity m_entityHandle { entt::null };
	entt::registry* m_registry { nullptr };
};

} // namespace enx
