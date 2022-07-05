#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/String.hpp>
#include <SFML/Window/ContextSettings.hpp>

namespace enx
{

namespace Screen
{
	void initialize(sf::RenderWindow& window);
	void shutdown();

	void setWindowSize(const sf::Vector2u& size);
	sf::Vector2u getWindowSize();

	void setWindowStyle(sf::Uint32 style);
	const std::vector<sf::VideoMode>& getFullscreenModes();

	void setFullscreen(const sf::VideoMode& videoMode);
	void setWindowed();
	bool isFullscreen();

	void setSize(const sf::Vector2u& size);
	const sf::Vector2u& getSize();

	void setTitle(const sf::String &title);
	const sf::String& getTitle();

	void setContextSettings(const sf::ContextSettings &settings);
	const sf::ContextSettings& getContextSettings();

	void apply();

	sf::RenderWindow* getInstance();
};

}
