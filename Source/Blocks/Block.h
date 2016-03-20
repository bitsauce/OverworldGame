#pragma once

#include "Config.h"
#include "Constants.h"
#include "Blocks/BlockData.h"
#include "BlockEntities/BlockEntity.h"
#include "BlockEntities/BlockEntityData.h"

class Block
{
public:
	Block() :
		m_blockData(0),
		m_blockEntity(0)
	{
	}

	Block(const BlockID id) :
		m_blockData(BlockData::get(id)),
		m_blockEntity(0)
	{
	}

	Block(BlockEntity *blockEntity) :
		m_blockData(blockEntity->getData()),
		m_blockEntity(blockEntity)
	{
	}

	Block(BlockData *data) :
		m_blockData(data),
		m_blockEntity(0)
	{
	}

	BlockData *getBlockData() const
	{
		return m_blockData;
	}

	BlockEntity *getBlockEntity() const
	{
		return m_blockEntity;
	}

	operator BlockID() const
	{
		return m_blockData->getID();
	}

private:
	BlockData *m_blockData;
	BlockEntity *m_blockEntity;
};