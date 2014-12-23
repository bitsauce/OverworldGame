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

private:
	static void addGameObject(GameObject *object);
	static void removeGameObject(GameObject *object);

	static xd::SpriteBatch *s_spriteBatch;

	static list<GameObject*> s_gameObjects;
};

#endif // GAME_MANAGER_H