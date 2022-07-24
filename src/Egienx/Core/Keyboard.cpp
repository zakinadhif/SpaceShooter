#include "Keyboard.hpp"

#include <algorithm>

namespace enx
{

std::array<bool, sf::Keyboard::KeyCount> Keyboard::m_states {};
std::array<bool, sf::Keyboard::KeyCount> Keyboard::m_keysUp {};
std::array<bool, sf::Keyboard::KeyCount> Keyboard::m_keysDown {};

bool Keyboard::isKeyPressed(sf::Keyboard::Key key)
{
	return m_states[key];
}

bool Keyboard::isKeyUp(sf::Keyboard::Key key)
{
	return m_keysUp[key];
}

bool Keyboard::isKeyDown(sf::Keyboard::Key key)
{
	return m_keysDown[key];
}

void Keyboard::setKey(sf::Keyboard::Key key, bool pressed)
{
	m_states[key] = pressed;

	if (pressed) m_keysDown[key] = true;
	else m_keysUp[key] = true;
}

void Keyboard::clearKeyStates()
{
	std::fill(m_keysUp.begin(), m_keysUp.end(), false);
	std::fill(m_keysDown.begin(), m_keysDown.end(), false);
}

}
