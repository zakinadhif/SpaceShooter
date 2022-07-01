#pragma once

#include <SFML/Graphics.hpp>

class SettingsManager
{
public:
	static void initialize(sf::RenderWindow& window);
	static SettingsManager& get();

	void setFullscreen(bool value, std::size_t videoModeIndex = 0);
	bool isFullscreen() const;

	void setResolution(std::size_t videoModeIndex = 0);
	std::size_t getResolution() const;

private:
	SettingsManager(sf::RenderWindow& window);
	static SettingsManager* m_settingsManager;

	sf::RenderWindow* m_window = nullptr;
	bool m_fullscreen = false;
	sf::Vector2u m_sizeBeforeFullscreen = {800, 800};
	std::size_t m_videoModeIndex = 0;

public:
	SettingsManager(SettingsManager const&) = delete;
	void operator=(SettingsManager const&) = delete;
};
