#include "Core/Engine.hpp"
#include "Core/EntryPoint.hpp"

#include "GameStates/PlayState.hpp"

void initializeGameState(enx::Engine &engine)
{
	engine.attach<enx::PlayState>();
}
