#include "World/Components/IdentifierComponent.hpp"
#include "World/Components/TransformComponent.hpp"
#include "World/World.hpp"
#include "World/Systems.hpp"
#include "Utility/VectorConverter.hpp"
#include "Utility/Random.hpp"
#include "World/Components/Components.hpp"
#include "World/Components/GameStateComponent.hpp"
#include "World/Builders/AsteroidBuilder.hpp"
#include "World/Entity.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <Thor/Math/Trigonometry.hpp>
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

namespace astro
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

void clearShotAsteroids(entt::registry &registry)
{
	auto view = registry.view<AsteroidComponent>();

	auto& gameState = registry.ctx<GameStateComponent>();
	const float scoreBonus = 1.f;

	for (auto& entity : view)
	{
		const auto& ac = view.get<AsteroidComponent>(entity);

		if (ac.shouldBeDestroyed)
		{
			registry.destroy(entity);
			gameState.score += scoreBonus;
			spdlog::info("Player score: {}", gameState.score);
		}
	}
}

void clearCollidedBullets(entt::registry& registry)
{
	auto view = registry.view<BulletComponent>();

	for (auto& entity : view)
	{
		const auto& bc = view.get<BulletComponent>(entity);

		if (bc.shouldBeDestroyed)
		{
			registry.destroy(entity);
		}
	}
}

void spawnAsteroidsRandomly(World& world, AsteroidBuilder& builder, sf::FloatRect spawnArea)
{
	const float speedMin = 50.f;
	const float speedMax = 150.f;
	const float rotationSpeedMin = -30.f;
	const float rotationSpeedMax = 30.f;

	float x = zfge::Random::getFloat(spawnArea.left, spawnArea.left + spawnArea.width);
	float y = zfge::Random::getFloat(spawnArea.top, spawnArea.top + spawnArea.height);

	float velocityMagnitude = zfge::Random::getFloat(speedMin, speedMax);
	float velocityAngle = 0.0f;

	float rotationSpeed = zfge::Random::getFloat(rotationSpeedMin, rotationSpeedMax);

	enum Sides {Left, Right, Top, Bottom};
	Sides side = (Sides) zfge::Random::getInt(0, 3);

	switch (side)
	{
		case Left:
			x = spawnArea.left;
			velocityAngle += zfge::Random::getFloat(-90.f, 90.f);
			// velocityAngle += 0.f;
			break;
		case Right:
			x = spawnArea.left + spawnArea.width;
			velocityAngle += zfge::Random::getFloat(90.f, 270.f);
			// velocityAngle += 180.f;
			break;
		case Top:
			y = spawnArea.top;
			velocityAngle += zfge::Random::getFloat(180.f, 360.f);
			// velocityAngle += 270.f;
			break;
		case Bottom:
			y = spawnArea.top + spawnArea.height;
			velocityAngle += zfge::Random::getFloat(0.f, 180.f);
			// velocityAngle += 90.f;
			break;
	}

	float _velocityAngle = thor::toRadian(velocityAngle);
	sf::Vector2f velocity {
		cos(_velocityAngle) * velocityMagnitude,
		-sin(_velocityAngle) * velocityMagnitude
	};

	float _rotationSpeed = thor::toRadian(rotationSpeed);

	builder.setPosition({x, y});
	builder.setAngularVelocity(_rotationSpeed);
	builder.setLinearVelocity(velocity);

	builder.spawn();
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

void displayComponentInfo(IdentifierComponent* ic) {
	if (ImGui::TreeNode("Identifier Component")) {
		if (ic) {
			ImGui::LabelText("Name", "%s", ic->name.c_str());
			ImGui::LabelText("Type", "%d", (int) ic->type);
		}
		ImGui::TreePop();
	}
}
void displayComponentInfo(TransformComponent*) {}
void displayComponentInfo(MeshComponent*) {}
void displayComponentInfo(OwningMeshComponent*) {}
void displayComponentInfo(NativeScriptComponent*) {}
void displayComponentInfo(BulletComponent*) {}
void displayComponentInfo(AsteroidComponent*) {}

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

	auto& selectedEntity = registry.ctx<GameStateComponent>().selectedEntity;

	registry.each([&](entt::entity entity) {
		if (auto ic = registry.try_get<IdentifierComponent>(entity)) {
			if (ImGui::Selectable(ic->name.c_str(), selectedEntity == (uint32_t) entity)) {
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
	const uint32_t selectedEntity = registry.ctx<GameStateComponent>().selectedEntity;

	ImGui::Begin("Component Inspector");

	entt::entity entity = (entt::entity) selectedEntity;

	if (registry.valid(entity)) {
		const auto componentPointers = getComponentPointers<
			IdentifierComponent,
			TransformComponent,
			MeshComponent,
			OwningMeshComponent,
			NativeScriptComponent,
			BulletComponent,
			AsteroidComponent
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

} // namespace astro
