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
	~BlockEntity();

	virtual void update(const float dt)
	{
	}

	virtual void draw(SpriteBatch *spriteBatch, const float alpha)
	{
	}

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

	const BlockEntityData *getData()
	{
		return m_data;
	}

private:
	const Vector2i m_position;
	const BlockEntityData *m_data;
	
protected:
	World * const m_world;
};

#endif // STATIC_ENTITY_H