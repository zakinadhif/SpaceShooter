#include "Engine.hpp"

extern void initializeGameState(enx::Engine& engine);

int main()
{
	enx::Engine engine;
	initializeGameState(engine);
	engine.initialize();
	engine.run();
}
