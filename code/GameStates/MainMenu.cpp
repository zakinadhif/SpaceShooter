#include "GameStates/MainMenu.hpp"

#include "GameStateManager.hpp"
#include "GameStates/PlayState.hpp"

#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Label.hpp>

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

	setVersionLabel("0.00.1");

	tgui::Button::Ptr startButton = m_gui.get<tgui::Button>("StartButton");
	tgui::Button::Ptr exitButton = m_gui.get<tgui::Button>("ExitButton");

	startButton->onClick([this](){
		m_gameStateManager.push<PlayState>(m_gameStateManager, m_mainWindow);
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
