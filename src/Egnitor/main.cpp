#include "Core/Engine.hpp"
#include "Core/EntryPoint.hpp"

#include "GameStates/PlayState.hpp"

void initializeGameState(enx::Engine &engine)
{
	engine.setInitialWindowSize({800u, 600u});
	engine.setInitialWindowTitle("Egnitor - Egienx Editor");
	engine.attach<enx::PlayState>();
}
