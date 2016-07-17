#ifndef IN_GAME_STATE_H
#define IN_GAME_STATE_H

#include "Config.h"
#include "GameState.h"
#include "Game/Scene.h"
#include "Gui/Canvas.h"

class World;
class Connection;

class InGameState : public GameState
{
public:
	InGameState(OverworldGame *game, Connection *connection);

	void onEnter();
	void onLeave();
	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);

private:
	World *m_world;
	Connection *m_connection;
};

#endif // IN_GAME_STATE_H