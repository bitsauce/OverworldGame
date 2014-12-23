#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <x2d/x2d.h>

enum DrawOrder;

class GameObject
{
	friend class GameManager;
public:
	GameObject(DrawOrder depth);
	virtual ~GameObject();

	virtual void update() { }
	virtual void draw(xd::SpriteBatch*) { }

private:
	const DrawOrder m_depth;
};

#endif // GAME_OBJECT_H