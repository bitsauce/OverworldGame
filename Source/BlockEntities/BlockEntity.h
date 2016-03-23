#ifndef STATIC_ENTITY_H
#define STATIC_ENTITY_H

#include "Config.h"
#include "Constants.h"

class World;
class Block;
class BlockEntityData;

class NeighborChangedEvent
{
public:
	NeighborChangedEvent(const int dx, const int dy, const Block *oldBlock, const Block *newBlock) :
		m_dx(dx),
		m_dy(dy),
		m_oldBlock(oldBlock),
		m_newBlock(newBlock)
	{
	}

	int getDx() const
	{
		return m_dx;
	}

	int getDy() const
	{
		return m_dy;
	}

	const Block *getOldBlock() const
	{
		return m_oldBlock;
	}

	const Block *getNewBlock() const
	{
		return m_newBlock;
	}

private:
	const int m_dx, m_dy;
	const Block *m_oldBlock;
	const Block *m_newBlock;
};

class BlockEntity
{
public:
	BlockEntity(World *world, const int x, const int y, const BlockEntityData *data);
	~BlockEntity();

	//virtual void onTick(const float dt);
	/*
	virtual void onInteract()
	{
		if(m_open) {
			m_open = false;
			m_frameIndex = 0.0f;
		}
		else {
			m_open = true;
			m_frameIndex = 1.0f;
		}
	}
	*/

	virtual void onNeighbourChanged(NeighborChangedEvent *e) { }

	void getVertices(Vertex *vertices, uint *indices, int i);

	Vector2I getPosition() const
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

	const BlockEntityData *getData() const
	{
		return m_data;
	}

	BlockEntityData *getData()
	{
		return const_cast<BlockEntityData*>(m_data);
	}

private:
	const Vector2I m_position;
	const BlockEntityData *m_data;
	
protected:
	World * const m_world;
};

#endif // STATIC_ENTITY_H