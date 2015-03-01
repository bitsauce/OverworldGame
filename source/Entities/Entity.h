#ifndef ENTITY_H
#define ENTITY_H

#include "Game/GameObject.h"

enum DrawOrder;

class Entity : public GameObject
{
public:
	Entity(const DrawOrder o) : GameObject(o) {}
};

#endif // ENTITY_H