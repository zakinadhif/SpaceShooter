#include "GameStates/PlayState.hpp"

namespace enx
{

PlayState::PlayState(enx::GameStateManager& gameStateManager, sf::RenderTarget& mainWindow)
	: m_gameStateManager(gameStateManager)
	, m_editorScene(std::make_unique<Scene>(mainWindow))
	, m_sceneInspectorPanel(*this)
{
	Entity e = m_editorScene->createEntity("ayam");
	auto& rb = e.addComponent<RigidbodyComponent>();
	auto& box = e.addComponent<BoxColliderComponent>();
	box.size = {2.f, 2.f};
	rb.type = b2_staticBody;
	e.getComponent<TransformComponent>().setPosition({0.f, 0.f});

	m_activeScene = Scene::clone(*m_editorScene);

	m_activeScene->startPhysics();
	m_sceneInspectorPanel.setContext(*m_activeScene);
}

void PlayState::setScenePlay()
{
	if (m_sceneState == SceneState::Simulate)
		setSceneStop();

	m_sceneState = SceneState::Runtime;

	m_activeScene = Scene::clone(*m_editorScene);
	m_activeScene->startPhysics();

	m_sceneInspectorPanel.setContext(*m_activeScene);
}

void PlayState::setSceneSimulate()
{
	if (m_sceneState == SceneState::Simulate)
		setSceneStop();

	m_sceneState = SceneState::Runtime;

	m_activeScene = Scene::clone(*m_editorScene);
	m_activeScene->startPhysics();

	m_sceneInspectorPanel.setContext(*m_activeScene);
}

void PlayState::setSceneStop()
{
	if (m_sceneState == SceneState::Runtime)
		m_activeScene->stopPhysics();
	else if (m_sceneState == SceneState::Simulate)
		m_activeScene->stopPhysics();

	m_sceneState = SceneState::Edit;

	m_activeScene = Scene::clone(*m_editorScene);

	m_sceneInspectorPanel.setContext(*m_activeScene);
}

void PlayState::handleEvent(sf::Event event)
{
	m_activeScene->handleEvent(event);

	switch (event.type)
	{
		case sf::Event::KeyReleased:
			if (event.key.code == sf::Keyboard::Key::Escape)
				m_gameStateManager.pop();
			break;
		case sf::Event::Closed:
			m_gameStateManager.clear();
			break;
		default:
			break;
	}
}

void PlayState::update(float deltaTime)
{
	switch (m_sceneState)
	{
		case SceneState::Edit:
			break;
		case SceneState::Simulate:
			break;
		case SceneState::Runtime:
			m_activeScene->updateScripts(deltaTime);
			break;
	}
}

void PlayState::fixedUpdate(float deltaTime)
{
	switch (m_sceneState)
	{
		case SceneState::Edit:
			break;
		case SceneState::Simulate:
			m_activeScene->fixedUpdatePhysics(deltaTime);
			break;
		case SceneState::Runtime:
			m_activeScene->fixedUpdateScripts(deltaTime);
			m_activeScene->fixedUpdatePhysics(deltaTime);
			break;
	}
}

void PlayState::imGuiDraw()
{
	m_sceneInspectorPanel.draw();
}

void PlayState::draw(sf::RenderTarget& target) const
{
	m_activeScene->draw(target, sf::RenderStates::Default);
}

}
