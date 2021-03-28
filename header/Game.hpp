#pragma once

#include "GameStateManager.hpp"

namespace astro
{

class Game
{
public:
	Game();

	void run();
private:
	void update(float deltaTime);
	void draw();

	zfge::GameStateManager gameStateManager;
};

}
