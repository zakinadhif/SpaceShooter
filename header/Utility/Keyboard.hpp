#pragma once

#include <SFML/Window/Keyboard.hpp>

#include <array>

namespace astro
{

class Keyboard
{
public:
	Keyboard() = delete;

	static bool isKeyPressed(sf::Keyboard::Key key);

private:
	static void setKey(sf::Keyboard::Key key, bool pressed);

	static std::array<bool, static_cast<std::size_t>(sf::Keyboard::Key::KeyCount)> m_states;

	friend class Game;
};

}
