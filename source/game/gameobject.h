#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Config.h"

enum Priority;

class GameObject
{
	friend class GameManager;
public:
	GameObject(Priority depth);
	virtual ~GameObject();

	virtual void update() { }
	virtual void draw(xd::SpriteBatch*) { }

private:
	const Priority m_depth;
};

#endif // GAME_OBJECT_H