#pragma once

#include <SFML/Window/Keyboard.hpp>

#include <array>

namespace enx
{

class Keyboard
{
public:
	Keyboard() = delete;

	static bool isKeyPressed(sf::Keyboard::Key key);

	static bool isKeyUp(sf::Keyboard::Key key);
	static bool isKeyDown(sf::Keyboard::Key key);

private:
	static void setKey(sf::Keyboard::Key key, bool pressed);
	static void clearKeyStates();

	static std::array<bool, sf::Keyboard::Key::KeyCount> m_states;
	static std::array<bool, sf::Keyboard::Key::KeyCount> m_keysDown;
	static std::array<bool, sf::Keyboard::Key::KeyCount> m_keysUp;

	friend class Engine;
};

}
