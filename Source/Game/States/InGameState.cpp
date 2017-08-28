#include "InGameState.h"
#include "Game/Game.h"
#include "Gui/GameOverlay/GameOverlay.h"

InGameState::InGameState(Overworld *game, World *world) :
	GameState(GAME_STATE_IN_GAME, false),
	m_world(world)
{
	addChildLast(game->getGameOverlay());
	addChildLast(m_world);
	if(game->getServer()) addChildLast(game->getServer());
	if(game->getClient()) addChildLast(game->getClient());
}

void InGameState::onTransitionBegin()
{
}
