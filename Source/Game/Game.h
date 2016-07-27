#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Config.h"
#include "Constants.h"

#include "Scene.h"
#include "Debug.h"
#include "Commander.h"

#include "Gui/Canvas.h"
#include "World/World.h"
#include "Networking/Client.h"
#include "Networking/Server.h"
#include "Entities/Pawn.h"

class GameOverlay;
class GameState;

class OverworldGame : public Game
{
public:
	OverworldGame();

	void onStart(GameEvent *e);
	void onEnd(GameEvent *e);
	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);

	Debug *getDebug() const
	{
		return m_debug;
	}

	World *getWorld() const
	{
		return m_world;
	}

	GameOverlay *getGameOverlay() const
	{
		return m_gameOverlay;
	}

	Commander *getCommander() const
	{
		return m_commander;
	}

	Canvas *getCanvas() const
	{
		return m_canvas;
	}

	void pushState(GameState *state);
	void popState();
	GameState *peekState(int level = 0);

	void takeScreenshot(KeyEvent *e)
	{
		if(e->getType() != KeyEvent::DOWN) return;
		m_takeScreenshot = true;
	}

	/*void toggleFullscreen(int action)
	{
		if(e->getType() != KeyEvent::DOWN) return;
		Window::setFullScreen(!Window::getFullScreen());
	}*/

private:
	void initKeybindings();

	bool m_takeScreenshot;

	// Game modules
	Canvas *m_canvas;
	Commander *m_commander;
	Debug *m_debug;
	World *m_world;
	GameOverlay *m_gameOverlay;
	SpriteBatch *m_spriteBatch;

	//list<GameState*> m_states;

	//Server *m_server;
	//Client *m_client;
};

#endif // GAME_MANAGER_H