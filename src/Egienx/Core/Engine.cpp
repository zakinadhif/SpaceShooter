#include "Engine.hpp"

#include "Core/Screen.hpp"
#include "Keyboard.hpp"
#include "Screen.hpp"

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

	const sf::Time fixedUpdateInterval = sf::seconds(1.0f / 60.0f);

	sf::Clock timer;
	sf::Time elapsed = sf::Time::Zero;
	sf::Time lag = sf::Time::Zero;

	while (m_window.isOpen() && !m_gameStateManager.isEmpty())
	{
		enx::GameState& currentState = m_gameStateManager.peek();

		elapsed = timer.restart();
		lag += elapsed;

		handleEvents();

		currentState.update(elapsed.asSeconds());

		while (lag >= fixedUpdateInterval)
		{
			lag -= fixedUpdateInterval;
			currentState.fixedUpdate(fixedUpdateInterval.asSeconds());
		}

		sf::CircleShape shape;
		shape.setRadius(100);

		m_window.clear();

		currentState.draw(m_window);

		ImGui::SFML::Update(m_window, elapsed);
		ImGui::DockSpaceOverViewport();
		ImGui::Begin("Engine Loop Stats");
		ImGui::LabelText("FPS", "%f", 1 / elapsed.asSeconds());
		ImGui::LabelText("Frame Time", "%f", elapsed.asSeconds());
		ImGui::End();

		currentState.imGuiDraw();
		ImGui::SFML::Render(m_window);

		m_window.display();

		m_gameStateManager.update();
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
