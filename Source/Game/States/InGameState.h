#ifndef IN_GAME_STATE_H
#define IN_GAME_STATE_H

#include "Config.h"
#include "GameState.h"
#include "Gui/Gui.h"

class World;
class Connection;

class InGameState : public GameState
{
public:
	InGameState(Overworld *game, World *world);

	void onTransitionBegin();

private:
	World *m_world;
};

#endif // IN_GAME_STATE_H