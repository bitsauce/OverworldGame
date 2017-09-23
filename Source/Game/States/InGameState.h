#ifndef IN_GAME_STATE_H
#define IN_GAME_STATE_H

#include "Config.h"
#include "GameState.h"
#include "Gui/Gui.h"
#include "InGameDebug.h"

class World;
class Connection;
class GameOverlay;

class InGameState : public GameState
{
public:
	InGameState(Overworld *game, World *world);

	void onTransitionBegin();

	World *getWorld() const
	{
		return m_world;
	}

	GameOverlay *getGameOverlay() const
	{
		return m_gameOverlay;
	}


private:
	GameOverlay *m_gameOverlay;
	World *m_world;
	InGameDebug *m_debug;
};

#endif // IN_GAME_STATE_H