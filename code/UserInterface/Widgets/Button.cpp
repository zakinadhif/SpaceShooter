#include "UserInterface/Widgets/Button.hpp"

#include <SFML/Graphics.hpp>

namespace astro
{

bool Button::initialized = false;
sf::Font Button::font;

Button::Button(const sf::Vector2f& position, const sf::Vector2f& size)
	: m_size{ size }
{
	if (!initialized)
	{
		font.loadFromFile("assets/munro.ttf");
		initialized = true;
	}

	m_text.setFont(font);
	m_text.setFillColor(sf::Color::Green);
	m_buttonFrame.setFillColor(sf::Color::Black);
	m_buttonFrame.setOutlineColor(sf::Color::Green);
	m_buttonFrame.setOutlineThickness(2);

	setPosition(position);
	update();
}

void Button::setText(const std::string& string)
{
	m_text.setString(string);
	update();
}

void Button::setSize(const sf::Vector2f& size)
{
	m_size = size;
	update();
}

void Button::handleEvent(sf::Event event)
{
	if (event.type == event.MouseButtonReleased)
	{
		sf::FloatRect buttonRect(getPosition(), m_size);
		sf::Vector2f mousePosition(event.mouseButton.x, event.mouseButton.y);

		if (buttonRect.contains(mousePosition))
		{
			if (m_onClick)
			{
				m_onClick();
			}
		}
	}
}

void Button::update()
{
	m_buttonFrame.setSize(m_size);

	sf::FloatRect textBounds = m_text.getLocalBounds();
	m_text.setPosition({(m_size.x - textBounds.width) / 2, (m_size.y - textBounds.height) / 2});
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(m_buttonFrame, states);
	target.draw(m_text, states);
}

}
