#include "SceneInspectorPanel.hpp"

#include "Core/Time.hpp"
#include "Scene/Systems.hpp"
#include "GameStates/PlayState.hpp"

#include <imgui.h>

namespace enx
{

SceneInspectorPanel::SceneInspectorPanel(PlayState& editor)
{
	m_editor = &editor;
}

SceneInspectorPanel::SceneInspectorPanel(PlayState& editor, Scene& scene)
{
	m_editor = &editor;
	setContext(scene);
}

void SceneInspectorPanel::setContext(Scene &scene)
{
	m_scene = &scene;
}

void SceneInspectorPanel::draw()
{
	if (!m_scene) return;

	const auto& deltaTime = Time::getDeltaTime();
	ImGui::Begin("Engine Loop Stats");
	ImGui::LabelText("FPS", "%f", 1 / deltaTime.asSeconds());
	ImGui::LabelText("Frame Time", "%f", deltaTime.asSeconds());
	ImGui::End();

	displayComponentInspector(m_scene->m_registry);
	displayEntityList(m_scene->m_registry);
}

}
