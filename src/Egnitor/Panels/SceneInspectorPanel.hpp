#pragma once

#include "Scene/Scene.hpp"

namespace enx
{

class PlayState;

// SceneInspectorPanel :: Dear ImGUI Panel
//
// Displays:
// - FPS
// - Frame Time
// - Box2D Debug Draw UI
class SceneInspectorPanel
{
public:
	SceneInspectorPanel(PlayState& editor);
	SceneInspectorPanel(PlayState& editor, Scene& scene);

	void setContext(Scene& scene);

	// NOTE: Context must reference a valid instance if called
	void draw();
private:
	Scene* m_scene = nullptr;
	PlayState* m_editor = nullptr;
};

}
