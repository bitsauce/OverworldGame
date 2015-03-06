#ifndef ENTITY_H
#define ENTITY_H

#include "Config.h"
#include "Game/GameObject.h"

enum DrawOrder;

class Entity : public GameObject
{
public:
	Entity(const DrawOrder o) : GameObject(o) {}

	virtual Vector2 getPosition() const = 0;
	virtual Vector2 getSize() const = 0;
	virtual Vector2 getCenter() const { return getPosition() + getSize() * 0.5f; }

private:
	//EntityID id;
};

#endif // ENTITY_H