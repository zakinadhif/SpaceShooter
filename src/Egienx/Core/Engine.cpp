#include "Engine.hpp"

#include "Core/Screen.hpp"
#include "Keyboard.hpp"
#include "Screen.hpp"
#include "Time.hpp"

#include <imgui.h>
#include <imgui-SFML.h>
#include <spdlog/spdlog.h>

namespace enx
{

Engine::Engine()
{
}

void Engine::setInitialWindowSize(const sf::Vector2u &size)
{
	m_initialWindowSize = size;
}

void Engine::setInitialWindowTitle(const sf::String &title)
{
	m_initialWindowTitle = title;
}

void Engine::initialize()
{
	Screen::initialize(m_window);
	Screen::setSize(m_initialWindowSize);
	Screen::setTitle(m_initialWindowTitle);
	Screen::apply();

	m_window.setKeyRepeatEnabled(false);
	m_window.setFramerateLimit(60);

	if (!ImGui::SFML::Init(m_window)) {
		spdlog::error("[Core::Engine::initialize] Couldn't initialize ImGui-SFML");
		exit(-1);
	}

	auto& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void Engine::run()
{
	while (m_window.isOpen() && m_scene.get())
	{
		sf::Time deltaTime = Time::getDeltaTime();

		ImGui::SFML::Update(m_window, deltaTime);
		ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);

		handleEvents();

    m_scene->updateScripts(deltaTime.asSeconds());

		while (Time::shouldFixedUpdate())
		{
			m_scene->fixedUpdateScripts(Time::getFixedDeltaTime().asSeconds());
		}

		m_window.clear();

		m_scene->draw(m_window, sf::RenderStates::Default);

		ImGui::SFML::Render(m_window);

		m_window.display();

		Time::markEndOfFrame();
		Keyboard::clearKeyStates();
	}

	if (m_window.isOpen())
		m_window.close();
}

void Engine::handleEvents()
{
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

		if (shouldPass) m_scene->handleEvent(event);
	}
}

Engine::~Engine()
{
	ImGui::SFML::Shutdown();
}

}
