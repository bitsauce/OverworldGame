#include "BlockEntityData.h"
#include "Constants.h"
#include "Terrain/Terrain.h"
#include "RedCurrantBush.h"

vector<BlockEntityData*> BlockEntityData::s_blockEntityData(BLOCK_ENTITY_COUNT);
Terrain *BlockEntityData::s_terrain = nullptr;

void BlockEntityData::init(Terrain *terrain)
{
	s_terrain = terrain;

	s_blockEntityData[BLOCK_ENTITY_RED_CURRANT_BUSH] = new BlockEntityData(4, 2, NEED_FLOOR, function<void(int, int)>(RedCurrantBush::Factory));
}

BlockEntityData::BlockEntityData(const int width, const int height, const PlacementRule rule, function<void(int, int)> factory) :
	m_width(width),
	m_height(height),
	m_placementRule(rule),
	m_factory(factory)
{
}

bool BlockEntityData::tryPlace(const int x, const int y)
{
	for(int i = 0; i < m_height; ++i)
	{
		for(int j = 0; j < m_width; ++j)
		{
			if(s_terrain->getBlockAt(x + j, y + i, TERRAIN_LAYER_MIDDLE) != BLOCK_EMPTY)
			{
				return false;
			}
		}
	}

	if(m_placementRule & NEED_FLOOR != 0)
	{
		for(int i = 0; i < m_width; ++i)
		{
			if(s_terrain->getBlockAt(x + i, y + m_height, TERRAIN_LAYER_MIDDLE) == BLOCK_EMPTY)
			{
				return false;
			}
		}
	}

	//switch(m_placementRule) {
	// case NEED_FLOOR:
	// case NEED_WALL:
	// case NEED_ROOF:
	// case NEED_BACKGROUND:
	// }

	for(int i = 0; i < m_height; ++i)
	{
		for(int j = 0; j < m_width; ++j)
		{
			s_terrain->setBlockAt(x + j, y + i, BLOCK_ENTITY, TERRAIN_LAYER_MIDDLE);
		}
	}
	m_factory(x, y);
	return true;
}

BlockEntityData &BlockEntityData::get(const BlockEntityID id)
{
	return *s_blockEntityData[id];
}