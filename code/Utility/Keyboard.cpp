#include "Utility/Keyboard.hpp"

namespace astro
{

std::array<bool, static_cast<std::size_t>(sf::Keyboard::KeyCount)> Keyboard::m_states {};

bool Keyboard::isKeyPressed(sf::Keyboard::Key key)
{
	return m_states[key];
}

void Keyboard::setKey(sf::Keyboard::Key key, bool pressed)
{
	m_states[key] = pressed;
}

}
