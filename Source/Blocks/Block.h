#pragma once

#include "Config.h"
#include "Constants.h"
#include "Blocks/BlockData.h"
#include "BlockEntities/BlockEntity.h"
#include "BlockEntities/BlockEntityData.h"

class ChunkBlock
{
public:
	ChunkBlock() :
		m_blockData(BlockData::get(BLOCK_EMPTY)),
		m_blockEntity(0)
	{
	}

	void setBlockID(const BlockID id)
	{
		m_blockData = BlockData::get(id);
	}

	void setBlockEntity(BlockEntity *entity)
	{
		m_blockEntity = entity;
	}

	void setBlockData(BlockData *data)
	{
		m_blockData = data;
	}

	BlockID getBlockID() const
	{
		return m_blockData->getID();
	}

	BlockData *getBlockData() const
	{
		return m_blockData;
	}

	BlockEntity *getBlockEntity() const
	{
		return m_blockEntity;
	}

	operator BlockEntity* () const
	{
		return m_blockEntity;
	}

	operator BlockData* () const
	{
		return m_blockData;
	}

	operator BlockID () const
	{
		return m_blockData->getID();
	}

	bool isEmpty() const
	{
		return *this == BLOCK_EMPTY && m_blockEntity == 0;
	}

private:
	BlockData *m_blockData;
	BlockEntity *m_blockEntity;
};