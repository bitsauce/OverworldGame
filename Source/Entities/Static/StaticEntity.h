#ifndef STATIC_ENTITY_H
#define STATIC_ENTITY_H

#include "Config.h"
#include "../Entity.h"

class World;

class StaticEntity : public Entity
{
public:
	StaticEntity(World *world, int x, int y, int w, int h, const EntityID id);

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
	Vector2i m_position;
	int m_refCount;
};

#endif // STATIC_ENTITY_H