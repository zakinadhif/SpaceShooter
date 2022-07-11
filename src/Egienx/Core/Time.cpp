#include "Time.hpp"

namespace
{
	sf::Time s_lastTime = {};
	sf::Time s_lag = {};
	sf::Time s_deltaTime = {};
}

namespace enx
{
	sf::Clock Time::timer {};
	sf::Time Time::fixedDeltaTime = sf::seconds(1.f / 60.f);

	sf::Time Time::getTime() { return timer.getElapsedTime(); }
	sf::Time Time::getDeltaTime() { return s_deltaTime; }
	sf::Time Time::getFixedDeltaTime() { return fixedDeltaTime; }

	void Time::markEndOfFrame() {
		auto currentTime = timer.getElapsedTime();

		s_deltaTime = currentTime - s_lastTime;
		s_lag += s_deltaTime;

		s_lastTime = currentTime;
	}

	bool Time::shouldFixedUpdate() {
		if (s_lag >= fixedDeltaTime) {
			s_lag -= fixedDeltaTime;
			return true;
		}

		return false;
	}

}
