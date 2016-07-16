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
		m_blockData(0),
		m_blockEntity(0)
	{
	}

	void setBlockEntity(BlockEntity *entity)
	{
		m_blockEntity = entity;
	}

	void setBlockData(const BlockData *data)
	{
		m_blockData = data;
	}

	const BlockData *getBlockData() const
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

	operator const BlockData* () const
	{
		return m_blockData;
	}

	bool isEmpty() const
	{
		return m_blockData == 0 && m_blockEntity == 0;
	}

private:
	const BlockData *m_blockData;
	BlockEntity *m_blockEntity;
};