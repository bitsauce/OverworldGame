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

	void setBlockEntity(BlockEntity *entity);
	void setBlockData(const BlockData *data);

	void setBlockDataByID(const BlockID id);

	const BlockData *getBlockData() const;
	BlockEntity *getBlockEntity() const;

	bool isEmpty() const;

private:
	const BlockData *m_blockData;
	BlockEntity *m_blockEntity;
};