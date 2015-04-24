#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Config.h"

class World;

class Game
{
public:
	Game();

	void main(GraphicsContext &context);
	void exit();
	void update(const float dt);
	void draw(GraphicsContext &context, const float alpha);

	void takeScreenshot() { m_takeScreenshot = true; }

private:
	SpriteBatch *m_spriteBatch;
	bool m_takeScreenshot;
	World *m_world;
	
	//Canvas *m_canvas;
	//SceneManager *m_sceneManager;
	//GameOverlay *m_gameOverlay;
	//Player *m_localPlayer;
};

#endif // GAME_MANAGER_H