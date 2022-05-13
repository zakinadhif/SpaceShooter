#include "GameStates/MainMenu.hpp"

#include "GameStateManager.hpp"
#include "GameStates/PlayState.hpp"
#include "Utility/SettingsManager.hpp"

#include <TGUI/AllWidgets.hpp>
#include <TGUI/String.hpp>
#include <sstream>
#include <string>

namespace astro
{

MainMenu::MainMenu(zfge::GameStateManager& gameStateManager, sf::RenderTarget& mainWindow)
	: m_gameStateManager(gameStateManager)
	, m_gui(mainWindow)
	, m_mainWindow(mainWindow)
{
	m_starfield.setDirection(Starfield::Direction::Down);
	m_starfield.setSize(m_mainWindow.getView().getSize());

	m_starfield.addLayer(10.f, 0.f);
	m_starfield.addLayer(6.f, 0.5f);

	m_gui.loadWidgetsFromFile("assets/main_menu.form");

	// Settings window
	auto settingsWindow = tgui::ChildWindow::create("Settings");
	{
		settingsWindow->setVisible(false);
		settingsWindow->setPosition("(parent.innersize - size) / 2");

		auto fullscreenCheckbox = tgui::CheckBox::create("Fullscreen");
		settingsWindow->add(fullscreenCheckbox, "FullscreenCheckbox");
		fullscreenCheckbox->onCheck([]() {
			SettingsManager::get().setFullscreen(true);
		});
		fullscreenCheckbox->onUncheck([]() {
			SettingsManager::get().setFullscreen(false);
		});

		auto resolutionDropdown = tgui::ComboBox::create();
		resolutionDropdown->setDefaultText("Resolution");
		settingsWindow->add(resolutionDropdown, "ResolutionDropdown");
		resolutionDropdown->setPosition(0, fullscreenCheckbox->getSize().y);

		std::ostringstream ss;
		int i = 0;
		for (const auto& fullscreenMode : sf::VideoMode::getFullscreenModes()) {
			ss.str(std::string());

			ss << fullscreenMode.width << 'x' << fullscreenMode.height << " BPP:" << fullscreenMode.bitsPerPixel;

			resolutionDropdown->addItem(ss.str(), std::to_string(i));
			++i;
		}

		resolutionDropdown->onItemSelect([=](tgui::String itemText, tgui::String itemId) {
			SettingsManager::get().setResolution(itemId.toUInt());
		});

		m_gui.add(settingsWindow, "SettingsWindow");
	}

	settingsWindow->onClosing([=](bool* abort) {
		*abort = true;
		settingsWindow->setVisible(false);
	});
	// End of settings window

	setVersionLabel("0.00.1");

	tgui::Button::Ptr startButton = m_gui.get<tgui::Button>("StartButton");
	tgui::Button::Ptr settingsButton = m_gui.get<tgui::Button>("SettingsButton");
	tgui::Button::Ptr exitButton = m_gui.get<tgui::Button>("ExitButton");

	startButton->onClick([this](){
		m_gameStateManager.push<PlayState>(m_gameStateManager, m_mainWindow);
	});

	settingsButton->onClick([=](){
		settingsWindow->setVisible(true);
	});

	exitButton->onClick([this](){
		m_gameStateManager.pop();
	});
}

void MainMenu::handleEvent(sf::Event event)
{
	m_gui.handleEvent(event);

	switch (event.type)
	{
		case sf::Event::KeyReleased:
			if (event.key.code == sf::Keyboard::Key::Escape)
				m_gameStateManager.pop();
			break;
		default:
			break;
	}
}

void MainMenu::update(float deltaTime)
{
	m_starfield.update(deltaTime);
}

void MainMenu::draw(sf::RenderTarget& target) const
{
	target.draw(m_starfield);
	m_gui.draw();
}

void MainMenu::setVersionLabel(const std::string& versionString)
{
	tgui::Label::Ptr versionLabel = m_gui.get<tgui::Label>("VersionLabel");
	versionLabel->setText(std::string("Version ") + versionString);
}

}
