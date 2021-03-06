#pragma once

#include "Config.h"
#include "Constants.h"

#define BLOCK_ENTITY_FACTORY(classname) \
	static BlockEntity *Factory(World *world, const int x, const int y, const BlockEntityData *data) \
	{ \
		return new classname(world, x, y, data); \
	}

class Block;
class World;
class BlockEntityData;

class NeighborChangedEvent
{
public:
	NeighborChangedEvent(const int dx, const int dy, const BlockID oldBlock, const BlockID newBlock) :
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

	BlockID getOldBlock() const
	{
		return m_oldBlock;
	}

	BlockID getNewBlock() const
	{
		return m_newBlock;
	}

private:
	const int m_dx, m_dy;
	const BlockID m_oldBlock;
	const BlockID m_newBlock;
};

class BlockEntity
{
public:
	BlockEntity(const Json::Value &attributes);
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

	virtual void onNeighbourChanged(NeighborChangedEvent *e);

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
	
protected:
	const BlockEntityData * const m_data;
	World * const m_world;
};