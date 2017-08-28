#pragma once

#include "Config.h"
#include "Constants.h"

#include "Debug.h"
#include "Commander.h"

#include "Gui/Gui.h"
#include "World/World.h"
#include "Networking/Client.h"
#include "Networking/Server.h"
#include "Entities/Pawn.h"

class GameOverlay;
class GameState;

class Overworld : public Game
{
public:
	Overworld();

	void onStart(GameEvent *e);
	void onEnd(GameEvent *e);
	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);

	void onEvent(Event *e);

	Debug *getDebug() const
	{
		return m_debug;
	}

	GameOverlay *getGameOverlay() const
	{
		return m_gameOverlay;
	}

	Commander *getCommander() const
	{
		return m_commander;
	}

	Server *getServer() const
	{
		return m_server;
	}

	Client *getClient() const
	{
		return m_client;
	}

	void pushState(GameState *state);
	bool popState();
	GameState *peekState(int level = 0);

	void takeScreenshot(InputEvent *e)
	{
		if(e->getType() != KeyEvent::DOWN) return;
		m_takeScreenshot = true;
	}

	/*void toggleFullscreen(int action)
	{
		if(e->getType() != KeyEvent::DOWN) return;
		Window::setFullScreen(!Window::getFullScreen());
	}*/

	static Overworld *Get() { return s_this; }

private:
	void initKeybindings();

	bool m_takeScreenshot;

	// Game modules
	Commander *m_commander;
	Debug *m_debug;
	GameOverlay *m_gameOverlay;
	SpriteBatch *m_spriteBatch;

	// Game state stack
	list<GameState*> m_states;

	// Transition variables
	float m_transitionTime;
	int m_transitionDirection;
	const float m_fadeTime;

	Server *m_server;
	Client *m_client;

	static Overworld *s_this;
};
