#ifndef BLOCK_ENTITY_H
#define BLOCK_ENTITY_H

#include "Config.h"
#include "Game/GameObject.h"

class Thing : public GameObject
{
public:
	Thing(const int x, const int y);

private:
	const int m_x, m_y;
};

#endif // BLOCK_ENTITY_H