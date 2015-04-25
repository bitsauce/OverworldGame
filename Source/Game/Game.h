#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Config.h"
#include "World/World.h"
#include "Networking/Client.h"
#include "Networking/Server.h"

class Debug;
class GameState;

class Game
{
public:
	Game();

	void main(GraphicsContext &context);
	void exit();
	void update(const float delta);
	void draw(GraphicsContext &context, const float alpha);

	SpriteBatch *getSpriteBatch() const { return m_spriteBatch; }
	Debug *getDebug() const { return m_debug; }
	World *getWorld() const { return m_world; }
	Client *getClient() const { return m_client; }
	Server *getServer() const { return m_server; }

	void pushState(GameState *state);
	void popState();
	GameState *peekState(int level = 0);

	void takeScreenshot() { m_takeScreenshot = true; }

private:
	SpriteBatch *m_spriteBatch;
	bool m_takeScreenshot;
	
	Debug *m_debug;
	World *m_world;
	list<GameState*> m_states;

	Server *m_server;
	Client *m_client;
	
	//Canvas *m_canvas;
	//GameOverlay *m_gameOverlay;
	//Player *m_localPlayer;
};

#endif // GAME_MANAGER_H