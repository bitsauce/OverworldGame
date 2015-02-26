#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Config.h"

class GameManager
{
	friend class GameObject;
public:
	static void main();
	static void exit();
	static void update();
	static void draw(GraphicsContext &context);

	static void takeScreenshot() { s_takeScreenshot = true; }

private:
	static void addGameObject(GameObject *object);
	static void removeGameObject(GameObject *object);

	static SpriteBatch *s_spriteBatch;
	static list<GameObject*> s_gameObjects;
	static bool s_takeScreenshot;
};

#endif // GAME_MANAGER_H