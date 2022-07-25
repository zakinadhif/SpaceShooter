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

void displayComponentInfo(TagComponent* tc) {
	if (ImGui::TreeNode("Tag Component")) {
		if (tc) {
			ImGui::LabelText("Name", "%s", tc->tag.c_str());
		}
		ImGui::TreePop();
	}
}
void displayComponentInfo(IDComponent* ic) {
	if (ImGui::TreeNode("ID Component")) {
		if (ic) {
			ImGui::LabelText("ID", "%i", ic->id);
		}
		ImGui::TreePop();
	}
}
void displayComponentInfo(TransformComponent* tc) {
	if (ImGui::TreeNode("Transform Component")) {
		if (tc) {
			ImGui::LabelText("Position", "%f, %f", tc->getPosition().x, tc->getPosition().y);
			ImGui::LabelText("Scale", "%f, %f", tc->getScale().x, tc->getScale().y);
			ImGui::LabelText("Rotation", "%f", tc->getRotation());
			ImGui::LabelText("Origin", "%f, %f", tc->getOrigin().x, tc->getOrigin().y);
		}
		ImGui::TreePop();
	}
}
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
		if (registry.all_of<IDComponent, TagComponent>(entity)) {
			auto& ic = registry.get<IDComponent>(entity);
			auto& tc = registry.get<TagComponent>(entity);

			std::string id = tc.tag + " #" + std::to_string(ic.id);
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

		displayComponentsInfo(componentPointers);
	} else {
		ImGui::Text("Selected entity is invalid");
	}

	ImGui::End();
}

} // namespace enx
