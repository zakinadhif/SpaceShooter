#pragma once

// Globals for fetching information about time

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

namespace enx
{

class Time
{
public:
	Time() = delete;

	static sf::Time getTime();
	static sf::Time getDeltaTime();
	static sf::Time getFixedDeltaTime();

private:
	static sf::Time fixedDeltaTime;
	static sf::Clock timer;

	static void markEndOfFrame();
	static bool shouldFixedUpdate();

	friend class Engine;
};

}
