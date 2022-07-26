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

void displayComponentForm(TagComponent& tc) {
	if (ImGui::TreeNode("Tag Component")) {
		ImGui::LabelText("Name", "%s", tc.tag.c_str());
		ImGui::TreePop();
	}
}

void displayComponentForm(IDComponent& ic) {
	if (ImGui::TreeNode("ID Component")) {
		ImGui::LabelText("ID", "%lu", ic.id);
		ImGui::TreePop();
	}
}

void displayComponentForm(TransformComponent& tc) {
	if (ImGui::TreeNode("Transform Component")) {
		ImGui::LabelText("Position", "%f, %f", tc.getPosition().x, tc.getPosition().y);
		ImGui::LabelText("Scale", "%f, %f", tc.getScale().x, tc.getScale().y);
		ImGui::LabelText("Rotation", "%f", tc.getRotation());
		ImGui::LabelText("Origin", "%f, %f", tc.getOrigin().x, tc.getOrigin().y);
		ImGui::TreePop();
	}
}

void displayComponentForm([[maybe_unused]] MeshComponent& mc) {}
void displayComponentForm([[maybe_unused]] OwningMeshComponent& omc) {}
void displayComponentForm([[maybe_unused]] NativeScriptComponent& nsc) {}
void displayComponentForm([[maybe_unused]] RigidbodyComponent& rbc) {}
void displayComponentForm([[maybe_unused]] BoxColliderComponent& bcc) {}
void displayComponentForm([[maybe_unused]] CircleColliderComponent& ccc) {}

template<typename... Component>
void displayComponentForms(
	entt::registry& registry,
	entt::entity entity
)
{
	([&]()
	{
		if (registry.all_of<Component>(entity))
		{
			displayComponentForm(registry.get<Component>(entity));
		}
	}(), ...);
}

template<typename... Component>
void displayComponentForms(
	ComponentGroup<Component...>,
	entt::registry& registry,
	entt::entity entity
)
{
	displayComponentForms<Component...>(registry, entity);
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
	const uint32_t selectedEntity = registry.ctx().at<GameStateComponent>().selectedEntity;

	ImGui::Begin("Component Inspector");

	entt::entity entity = (entt::entity) selectedEntity;

	if (registry.valid(entity)) {
		displayComponentForms(ImplComponents{}, registry, entity);
	} else {
		ImGui::Text("No entity selected");
	}

	ImGui::End();
}

} // namespace enx
