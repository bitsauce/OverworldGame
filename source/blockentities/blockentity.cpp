#include "blockentity.h"
#include "constants.h"
#include "game/world.h"
#include "terrain/terrain.h"

BlockEntity::BlockEntity(const uint width, const uint height) :
	GameObject(DRAW_ORDER_BLOCK_ENTITY),
	m_x(0),
	m_y(0),
	m_width(width),
	m_height(height),
	m_placed(false),
	m_placeable(false)
{
}

void BlockEntity::move(const int x, const int y)
{
	if(m_placed) return;

	m_x = x;
	m_y = y;

	m_placeable = true;
	for(int y = 0; y < m_height && m_placeable; ++y)
	{
		for(int x = 0; x < m_width && m_placeable; ++x)
		{
			BlockID id = World::getTerrain()->getBlockAt(m_x + x, m_y + y, TERRAIN_LAYER_SCENE);
			if(id >= BLOCK_OCCUPIED)
			{
				m_placeable = false;
			}
		}
	}

	moveEvent();
}

bool BlockEntity::place()
{
	if(m_placeable)
	{
		for(int y = 0; y < m_height && m_placeable; ++y)
		{
			for(int x = 0; x < m_width && m_placeable; ++x)
			{
				World::getTerrain()->setBlockAt(m_x + x, m_y + y, BLOCK_OCCUPIED, TERRAIN_LAYER_SCENE);
			}
		}

		m_placed = true;
		placeEvent();
		return true;
	}
	return false;
}