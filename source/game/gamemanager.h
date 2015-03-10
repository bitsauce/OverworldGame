#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Config.h"

class World;

class GameManager
{
public:
	static void main(GraphicsContext &context);
	static void exit();
	static void update(const float dt);
	static void draw(GraphicsContext &context, const float alpha);

	static void takeScreenshot() { s_takeScreenshot = true; }

private:
	static SpriteBatch *s_spriteBatch;
	static bool s_takeScreenshot;
	static World *m_world;
};

#endif // GAME_MANAGER_H