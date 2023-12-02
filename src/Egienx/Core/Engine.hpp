#pragma once

#include "Scene/Scene.hpp"

#include <SFML/Graphics.hpp>

namespace enx
{

class Engine
{
public:
	Engine();

	void setInitialWindowSize(const sf::Vector2u& size);
	void setInitialWindowTitle(const sf::String& title);
	void initialize();

	void run();

	~Engine();

private:
	void handleEvents();

	sf::Vector2u m_initialWindowSize {200u, 200u};
	sf::String m_initialWindowTitle {};

  std::unique_ptr<Scene> m_scene;
	sf::RenderWindow m_window;
};

}
