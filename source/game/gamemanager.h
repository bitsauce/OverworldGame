#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <x2d/x2d.h>

class GameManager
{
	friend class GameObject;
public:
	static void main();
	static void exit();
	static void update();
	static void draw(xd::GraphicsContext &context);

	static void takeScreenshot() { s_takeScreenshot = true; }

private:
	static void addGameObject(GameObject *object);
	static void removeGameObject(GameObject *object);

	static xd::SpriteBatch *s_spriteBatch;
	static list<GameObject*> s_gameObjects;
	static bool s_takeScreenshot;
};

#endif // GAME_MANAGER_H