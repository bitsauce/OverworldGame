#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Config.h"

class World;
class GameState
{
	friend class Game;
protected:
	GameState();

	virtual void update();
	virtual void draw();

	virtual void enter();
	virtual void leave();

private:
	bool m_isTransparent;
};

class Game
{
public:
	Game();

	void main(GraphicsContext &context);
	void exit();
	void update(const float dt);
	void draw(GraphicsContext &context, const float alpha);

	void takeScreenshot() { m_takeScreenshot = true; }

	void pushState(GameState *state);
	void popState();

private:
	SpriteBatch *m_spriteBatch;
	bool m_takeScreenshot;
	World *m_world;
	stack<GameState*> m_states;
	
	//Canvas *m_canvas;
	//SceneManager *m_sceneManager;
	//GameOverlay *m_gameOverlay;
	//Player *m_localPlayer;
};

#endif // GAME_MANAGER_H