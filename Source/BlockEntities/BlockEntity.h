#ifndef STATIC_ENTITY_H
#define STATIC_ENTITY_H

#include "Config.h"
#include "Constants.h"

class World;
class BlockEntityData;

class BlockEntity
{
public:
	BlockEntity(World *world, const int x, const int y, const BlockEntityData *data);

	Vector2i getPosition() const
	{
		return m_position;
	}

	int getX() const
	{
		return m_position.x;
	}

	int getY() const
	{
		return m_position.y;
	}

private:
	const Vector2i m_position;
	const BlockEntityData *m_data;
};

#endif // STATIC_ENTITY_H