#include "Scene/Components/Components.hpp"
#include "Scene/Components/RigidBodyComponent.hpp"
#include "Scene/Scene.hpp"
#include "Scene/Systems.hpp"
#include "Utility/VectorConverter.hpp"
#include "Core/Random.hpp"
#include "Scene/Components/Components.hpp"
#include "Scene/Entity.hpp"
#include "Patch/Thor.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <array>
#include <box2d/box2d.h>
#include <cstdint>
#include <entt/core/fwd.hpp>
#include <entt/entt.hpp>
#include <spdlog/spdlog.h>
#include <imgui-SFML.h>
#include <imgui.h>

#include <bitset>
#include <tuple>
#include <utility>

namespace enx
{

void drawEntities(const entt::registry& registry, sf::RenderTarget& target)
{
	const auto view = registry.view<const RigidBodyComponent>();

	for (const auto& entity : view)
	{
		const auto& rbc = registry.get<RigidBodyComponent>(entity);
		const TransformComponent* tc = registry.try_get<TransformComponent>(entity);

		sf::Transform transform;
		transform.translate(rbc.getPosition());
		transform.rotate(thor::toDegree(rbc.body->GetAngle()));

		if (tc)
		{
			const auto& graphicsTransform = tc->getTransform();
			transform *= graphicsTransform;
		}

		if (registry.all_of<MeshComponent>(entity))
		{
			const MeshComponent mesh = registry.get<MeshComponent>(entity);
			target.draw(mesh.vertices, mesh.size, mesh.type, sf::RenderStates(transform));
		}

		if (registry.all_of<OwningMeshComponent>(entity))
		{
			const OwningMeshComponent& mesh = registry.get<OwningMeshComponent>(entity);
			target.draw(mesh.vertices.data(), mesh.vertices.size(), mesh.type, sf::RenderStates(transform));
		}
	}
}

namespace
{

template<typename Component>
Component* fillTuple(
	entt::registry& registry,
	entt::entity entity
)
{
	Component* component = nullptr;

	if (registry.all_of<Component>(entity))
	{
		component = &registry.get<Component>(entity);
	}

	return component;
}

template<typename... Comps>
std::tuple<Comps*...> getComponentPointers(entt::registry& registry, entt::entity entity)
{
	return std::make_tuple(fillTuple<Comps>(registry, entity)...);
}

void displayComponentInfo(TagComponent* ic) {
	if (ImGui::TreeNode("Identifier Component")) {
		if (ic) {
			ImGui::LabelText("Name", "%s", ic->tag.c_str());
		}
		ImGui::TreePop();
	}
}
void displayComponentInfo(IDComponent*) {}
void displayComponentInfo(TransformComponent*) {}
void displayComponentInfo(MeshComponent*) {}
void displayComponentInfo(OwningMeshComponent*) {}
void displayComponentInfo(NativeScriptComponent*) {}

template <std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp), void>::type displayComponentsInfo(const std::tuple<Tp...>& t)
{

}

template <std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I < sizeof...(Tp), void>::type displayComponentsInfo(const std::tuple<Tp...>& t)
{
	displayComponentInfo(std::get<I>(t));
	displayComponentsInfo<I + 1, Tp...>(t);
}

} // namespace

void displayEntityList(entt::registry &registry) {
	ImGui::Begin("Entity List");

	auto& selectedEntity = registry.ctx().at<GameStateComponent>().selectedEntity;

	registry.each([&](entt::entity entity) {
		if (auto ic = registry.try_get<IDComponent>(entity)) {
			std::string id = std::to_string(ic->id);
			if (ImGui::Selectable(id.c_str(), selectedEntity == (uint32_t) entity)) {
				selectedEntity = (uint32_t) entity;
			}
		} else {
			std::string strbuf = "Unnamed Entity: ";
			strbuf += std::to_string((std::uint32_t) entity);
			if (ImGui::Selectable(strbuf.c_str(), selectedEntity == (uint32_t) entity)) {
				selectedEntity = (uint32_t) entity;
			}
		}
	});

	ImGui::End();
}

void displayComponentInspector(entt::registry& registry)
{
	const std::size_t componentCount = 7;
	const uint32_t selectedEntity = registry.ctx().at<GameStateComponent>().selectedEntity;

	ImGui::Begin("Component Inspector");

	entt::entity entity = (entt::entity) selectedEntity;

	if (registry.valid(entity)) {
		const auto componentPointers = getComponentPointers<
			IDComponent,
			TagComponent,
			TransformComponent,
			MeshComponent,
			OwningMeshComponent,
			NativeScriptComponent
		>(registry, entity);

		if (ImGui::TreeNode("Components", "Entity %d", (uint32_t) entity)) {
			displayComponentsInfo(componentPointers);
			ImGui::TreePop();
		}
	} else {
		ImGui::Text("Selected entity is invalid");
	}

	ImGui::End();
}

} // namespace enx
