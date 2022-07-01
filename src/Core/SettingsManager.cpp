#include "SettingsManager.hpp"

#include <cassert>
#include <iostream>

SettingsManager::SettingsManager(sf::RenderWindow& window)
	: m_window(&window)
{

}

void SettingsManager::initialize(sf::RenderWindow &window) {
	assert(!m_settingsManager && "FUCK: SettingsManager initialized twice!");
	m_settingsManager = new SettingsManager(window);
}

SettingsManager& SettingsManager::get() {
	assert(m_settingsManager);
	return *m_settingsManager;
}

SettingsManager* SettingsManager::m_settingsManager = nullptr;

// REAL CODE ===

void SettingsManager::setFullscreen(bool value, std::size_t videoModeIndex) {
	m_fullscreen = value;

	if (value) {
		m_sizeBeforeFullscreen = m_window->getSize();
		m_window->create(sf::VideoMode::getFullscreenModes()[videoModeIndex], "", sf::Style::Fullscreen);
	} else {
		m_window->create(sf::VideoMode(m_sizeBeforeFullscreen.x, m_sizeBeforeFullscreen.y), "", sf::Style::Default);
	}
}

bool SettingsManager::isFullscreen() const {
	return m_fullscreen;
}

void SettingsManager::setResolution(std::size_t videoModeIndex) {
	m_videoModeIndex = videoModeIndex;
	m_window->create(sf::VideoMode::getFullscreenModes().at(videoModeIndex), "", (isFullscreen() ? sf::Style::Fullscreen : sf::Style::Default));
}

std::size_t SettingsManager::getResolution() const {
	return m_videoModeIndex;
}
