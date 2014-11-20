#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <x2d/x2d.h>

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void update() { }
	virtual void draw(XBatch*) { }
};

class Game
{
	friend class GameObject;
public:
	static void update();
	static void draw();

private:
	static void addGameObject(GameObject *object);
	static void removeGameObject(GameObject *object);

	static list<GameObject*> s_gameObjects;
};

#endif // GAME_OBJECT_H