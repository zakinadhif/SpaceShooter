#include "Screen.hpp"

#include <SFML/Config.hpp>
#include <SFML/Window/ContextSettings.hpp>
#include <SFML/Window/WindowStyle.hpp>

#include <cassert>

namespace
{
	sf::RenderWindow* s_window { nullptr };
	sf::String s_title { "" };
	sf::ContextSettings s_contextSettings {};
	sf::Vector2u s_size { 600, 600 };
	sf::Uint32 s_style { sf::Style::Default };
	sf::VideoMode s_fullscreenVideoMode {};
	bool s_isFullscreen { false };
}

namespace enx::Screen
{
	void initialize(sf::RenderWindow& window)
	{
		s_window = &window;
	}

	void shutdown()
	{
		s_window = nullptr;
	}

	void setWindowSize(const sf::Vector2u& size)
	{
		s_window->setSize(size);
	}

	sf::Vector2u getWindowSize()
	{
		return s_window->getSize();
	}

	void setWindowStyle(sf::Uint32 style) { s_style = style; }

	const std::vector<sf::VideoMode>& getFullscreenModes()
	{
		return sf::VideoMode::getFullscreenModes();
	}

	void setFullscreen(const sf::VideoMode& videoMode)
	{
		// Check if videoMode is suitable for fullscreen
		assert(videoMode.isValid());

		s_isFullscreen = true;
		s_fullscreenVideoMode = videoMode;
	}

	void setWindowed() {
		s_isFullscreen = false;
	}

	bool isFullscreen() { return s_isFullscreen; }

	void setSize(const sf::Vector2u& size) { s_size = size; }
	const sf::Vector2u& getSize() { return s_size; }

	void setTitle(const sf::String &title) { s_title = title; }
	const sf::String& getTitle() { return s_title; }

	void setContextSettings(const sf::ContextSettings &settings) { s_contextSettings = settings; }
	const sf::ContextSettings& getContextSettings() { return s_contextSettings; }

	void apply()
	{
		if (s_isFullscreen)
		{
			s_window->create(s_fullscreenVideoMode, s_title, s_style | sf::Style::Fullscreen, s_contextSettings);
		}
		else
		{
			// Ignores sf::Style::Fullscreen attribute
			sf::Uint32 style = ~sf::Style::Fullscreen & s_style;

			sf::VideoMode currentDesktopMode = sf::VideoMode::getDesktopMode();
			s_window->create({s_size.x, s_size.y, currentDesktopMode.bitsPerPixel}, s_title, style, s_contextSettings);
		}
	}

	sf::RenderWindow* getInstance() { return s_window; }
}
