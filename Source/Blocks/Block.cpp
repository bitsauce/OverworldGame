#include "Block.h"

void Block::setBlockEntity(BlockEntity * entity)
{
	m_blockEntity = entity;
}

void Block::setBlockData(const BlockData *data)
{
	m_blockData = data;
}

void Block::setBlockDataByID(const BlockID id)
{
	m_blockData = BlockData::get(id);
}

const BlockData *Block::getBlockData() const
{
	return m_blockData;
}

BlockEntity *Block::getBlockEntity() const
{
	return m_blockEntity;
}

bool Block::isEmpty() const
{
	return m_blockData == 0 && m_blockEntity == 0;
}
