#include "BlockEntityData.h"
#include "Constants.h"
#include "Terrain/Terrain.h"
#include "RedCurrantBush.h"

vector<BlockEntityData*> BlockEntityData::s_blockEntityData(BLOCK_ENTITY_COUNT);
Terrain *BlockEntityData::s_terrain = nullptr;

void BlockEntityData::init(Terrain *terrain)
{
	s_terrain = terrain;

	s_blockEntityData[BLOCK_ENTITY_RED_CURRANT_BUSH] = new BlockEntityData(4, 2, function<void(int, int)>(RedCurrantBush::Factory));
}

BlockEntityData::BlockEntityData(const int width, const int height, function<void(int, int)> factory) :
	m_width(width),
	m_height(height),
	m_factory(factory)
{
}

bool BlockEntityData::tryPlace(const int x, const int y)
{
	bool placeable = true;
	for(int i = 0; i < m_height && placeable; ++i)
	{
		for(int j = 0; j < m_width && placeable; ++j)
		{
			if(s_terrain->getBlockAt(x + j, y + i, TERRAIN_LAYER_MIDDLE) != BLOCK_EMPTY)
			{
				placeable = false;
			}
		}
	}

	if(placeable)
	{
		for(int i = 0; i < m_height; ++i)
		{
			for(int j = 0; j < m_width; ++j)
			{
				s_terrain->setBlockAt(x + j, y + i, BLOCK_ENTITY, TERRAIN_LAYER_MIDDLE);
			}
		}
		m_factory(x, y);
	}
	else
	{
		return false;
	}
	return true;
}

BlockEntityData &BlockEntityData::get(const BlockEntityID id)
{
	return *s_blockEntityData[id];
}