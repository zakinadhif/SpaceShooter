#include "Engine.hpp"

#include "Core/Screen.hpp"
#include "Keyboard.hpp"
#include "Screen.hpp"
#include "Time.hpp"

#include <imgui.h>
#include <imgui-SFML.h>

namespace enx
{

Engine::Engine()
{
	Screen::initialize(m_window);
	Screen::setWindowSize({600, 600});
	Screen::setTitle("Egienx");
	Screen::apply();

	m_window.setKeyRepeatEnabled(false);
	m_window.setFramerateLimit(60);

	ImGui::SFML::Init(m_window);

	auto& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void Engine::run()
{
	m_gameStateManager.update();

	while (m_window.isOpen() && !m_gameStateManager.isEmpty())
	{
		sf::Time deltaTime = Time::getDeltaTime();
		enx::GameState& currentState = m_gameStateManager.peek();

		handleEvents();

		currentState.update(deltaTime.asSeconds());

		while (Time::shouldFixedUpdate())
		{
			currentState.fixedUpdate(Time::getFixedDeltaTime().asSeconds());
		}

		m_window.clear();

		currentState.draw(m_window);

		ImGui::SFML::Update(m_window, deltaTime);
		ImGui::DockSpaceOverViewport();
		ImGui::Begin("Engine Loop Stats");
		ImGui::LabelText("FPS", "%f", 1 / deltaTime.asSeconds());
		ImGui::LabelText("Frame Time", "%f", deltaTime.asSeconds());
		ImGui::End();

		currentState.imGuiDraw();
		ImGui::SFML::Render(m_window);

		m_window.display();

		m_gameStateManager.update();

		Time::markEndOfFrame();
		Keyboard::clearKeyStates();
	}

	if (m_window.isOpen())
		m_window.close();
}

void Engine::handleEvents()
{
	enx::GameState& currentState = m_gameStateManager.peek();
	ImGuiIO& io = ImGui::GetIO();

	for (sf::Event event; m_window.pollEvent(event);)
	{
		bool shouldPass = true;

		ImGui::SFML::ProcessEvent(event);

		switch(event.type) {
			case sf::Event::KeyPressed:
				if (io.WantCaptureKeyboard) {
					shouldPass = false;
				} else {
					Keyboard::setKey(event.key.code, true);
				}
				break;
			case sf::Event::KeyReleased:
				if (io.WantCaptureKeyboard) {
					shouldPass = false;
				} else {
					Keyboard::setKey(event.key.code, false);
				}
				break;
			case sf::Event::MouseButtonPressed:
				if (io.WantCaptureMouse) {
					shouldPass = false;
				}
				break;
			case sf::Event::MouseButtonReleased:
				if (io.WantCaptureMouse) {
					shouldPass = false;
				}
				break;
			default:
				break;
		}

		if (shouldPass) currentState.handleEvent(event);
	}
}

Engine::~Engine()
{
	ImGui::SFML::Shutdown();
}

}
