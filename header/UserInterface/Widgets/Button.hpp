#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Vector2.hpp>

#include <SFML/Window/Event.hpp>
#include <functional>
#include <string>

namespace astro
{

class Button final : public sf::Drawable, sf::Transformable
{
public:
	Button(const sf::Vector2f& position, const sf::Vector2f& size);

	void setText(const std::string& string);
	const sf::String& getText() const { return m_text.getString(); }

	void setSize(const sf::Vector2f& size);
	sf::Vector2f getSize() const { return m_size; }

	void onClick(std::function<void()> callback) { m_onClick = callback; }

	void handleEvent(sf::Event event);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	void update();

	sf::Vector2f m_size;

	sf::RectangleShape m_buttonFrame;
	sf::Text m_text;

	std::function<void()> m_onClick;

	static bool initialized;
	static sf::Font font;
};

}
