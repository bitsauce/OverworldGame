#include "BlockEntityData.h"
#include "Game.h"

vector<BlockEntityData*> BlockEntityData::s_blockEntityData(BLOCK_ENTITY_COUNT);
Terrain *BlockEntityData::s_terrain = nullptr;

void BlockEntityData::init(Terrain *terrain)
{
	s_terrain = terrain;
	
	s_blockEntityData[BLOCK_ENTITY_RED_CURRANT_BUSH] = new BlockEntityData(4, 2, NEED_FLOOR, ":/Sprites/BlockEntities/Bushes/RedCurrantBush.png", TextureRegion(0.0f, 0.0f, 1.0f, 0.5f), function<void(int, int)>(RedCurrantBush::Factory));
	s_blockEntityData[BLOCK_ENTITY_TORCH] = new BlockEntityData(1, 1, NEED_FLOOR | NEED_ROOF | NEED_WALLS | NEED_BACKGROUND, ":/Sprites/BlockEntities/LightSources/Torch.png", TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), function<void(int, int)>(Torch::Factory));
}

BlockEntityData::BlockEntityData(const int width, const int height, const int rule, const string &texture, const TextureRegion &textureRegion, function<void(int, int)> factory) :
	m_width(width),
	m_height(height),
	m_placementRule(rule),
	m_sprite(Sprite(ResourceManager::get<Texture2D>(texture))),
	m_factory(factory)
{
	m_sprite.setRegion(textureRegion, true);
}

bool BlockEntityData::canPlace(const int x, const int y)
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

	bool valid;
	if((m_placementRule & NEED_FLOOR) != 0)
	{
		// Check valid flooring
		valid = true;
		for(int i = 0; i < m_width && valid; ++i)
		{
			if(s_terrain->getBlockAt(x + i, y + m_height, TERRAIN_LAYER_MIDDLE) <= BLOCK_ENTITY)
			{
				valid = false;
			}
		}
		if(valid) return true;
	}
	
	if((m_placementRule & NEED_ROOF) != 0)
	{
		// Check valid roofing
		valid = true;
		for(int i = 0; i < m_width && valid; ++i)
		{
			if(s_terrain->getBlockAt(x + i, y - 1, TERRAIN_LAYER_MIDDLE) <= BLOCK_ENTITY)
			{
				valid = false;
			}
		}
		if(valid) return true;
	}
	
	if((m_placementRule & NEED_WALLS) != 0)
	{
		// Check valid walling
		valid = true;
		for(int i = 0; i < m_height && valid; ++i)
		{
			if(s_terrain->getBlockAt(x - 1, y + i, TERRAIN_LAYER_MIDDLE) <= BLOCK_ENTITY)
			{
				valid = false;
			}
		}
		if(valid) return true;
		
		valid = true;
		for(int i = 0; i < m_height && valid; ++i)
		{
			if(s_terrain->getBlockAt(x + m_width, y + i, TERRAIN_LAYER_MIDDLE) <= BLOCK_ENTITY)
			{
				valid = false;
			}
		}
		if(valid) return true;
	}
	
	if((m_placementRule & NEED_BACKGROUND) != 0)
	{
		// Check valid backgrounding
		valid = true;
		for(int j = 0; j < m_height && valid; ++j)
		{
			for(int i = 0; i < m_width && valid; ++i)
			{
				if(s_terrain->getBlockAt(x + i, y + j, TERRAIN_LAYER_BACK) <= BLOCK_ENTITY)
				{
					valid = false;
				}
			}
		}
		if(valid) return true;
	}

	// Not a valid placement
	return false;
}

bool BlockEntityData::tryPlace(const int x, const int y)
{
	if(canPlace(x, y))
	{
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
	return false;
}

BlockEntityData &BlockEntityData::get(const BlockEntityID id)
{
	return *s_blockEntityData[id];
}