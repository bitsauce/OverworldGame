#ifndef BLOCK_ENTITY_H
#define BLOCK_ENTITY_H

#include "Config.h"
#include "Constants.h"
#include "Game/GameObject.h"

class BlockEntity : public GameObject
{
public:
	BlockEntity(const int x, const int y) :
		GameObject(DRAW_ORDER_BLOCK_ENTITY),
		m_x(x),
		m_y(y)
	{
	}

private:
	const int m_x, m_y;
};

#endif // BLOCK_ENTITY_H