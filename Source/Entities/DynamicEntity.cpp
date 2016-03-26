#include "DynamicEntity.h"
#include "Constants.h"

#include "World/World.h"

DynamicEntity::DynamicEntity(World *world, const EntityID id) :
	Entity(world, id),
	m_acceleration(0.0f, 0.0f),
	m_velocity(0.0f, 0.0f),
	m_position(0.0f, 0.0f),
	m_lastPosition(0.0f, 0.0f),
	m_size(1.0f, 1.0f),
	m_rotation(45.0f),
	m_gravityScale(1.0f),
	m_allowRotation(false),
	m_contact(0),
	m_terrain(world->getTerrain())
{
}

void DynamicEntity::onTick(TickEvent *e)
{
	m_lastContact = m_contact;
	m_lastPosition = m_position;
	m_contact = 0; 
	m_acceleration.y += 2.0f * m_gravityScale;
	m_velocity += m_acceleration;
	m_acceleration.set(0.0f, 0.0f);

	if(m_allowRotation)
	{
		m_position.y += m_velocity.y;
		m_position.x += m_velocity.x;
		/*Vector2F points[4];
		m_sprite->getAABB(points);
		
		// Check down
		if(m_velocity.y > 0.0f)
		{
			for(uint i = 0; i < 4; ++i)
			{
				int x0 = floor(points[i].x/BLOCK_PXF);
				int y0 = floor(points[i].y/BLOCK_PXF);
				int v1 = floor((points[i].y+m_velocity.y)/BLOCK_PXF);

				for(int y = y0; y <= v1 && m_velocity.y > 0.0f; ++y)
				{
					if(m_terrain->getBlockAt(x0, y, WORLD_LAYER_MIDDLE) > BLOCK_ENTITY)
					{
						m_position.y = y * BLOCK_PXF - m_size.y;
						m_velocity.y = 0.0f;
					}
				}
			}
		}*/
	}
	else
	{
		// Apply vertical velocity
		m_position.y += m_velocity.y;

		// Check collision vertically
		if(m_velocity.y > 0.0f)
		{
			int x0 = (int)floor(m_position.x/BLOCK_PXF), x1 = (int)floor((m_position.x+m_size.x-1)/BLOCK_PXF);
			int y0 = (int)floor((m_position.y+m_size.y-1)/BLOCK_PXF), y1 = (int)floor((m_position.y+m_size.y-1+m_velocity.y)/BLOCK_PXF);
			for(int y = y0; y <= y1 && m_velocity.y > 0.0f; ++y)
			{
				for(int x = x0; x <= x1 && m_velocity.y > 0.0f; ++x)
				{
					if(m_terrain->isBlockAt(x, y, WORLD_LAYER_MIDDLE))
					{
						m_position.y = y * BLOCK_PXF - m_size.y;
						m_velocity.y = 0.0f;
						m_contact |= SOUTH;
					}
				}
			}
		}
		else if(m_velocity.y < 0.0f)
		{
			int x0 = (int)floor(m_position.x/BLOCK_PXF), x1 = (int)floor((m_position.x+m_size.x-1)/BLOCK_PXF);
			int y0 = (int)floor(m_position.y/BLOCK_PXF), y1 = (int)floor((m_position.y+m_velocity.y)/BLOCK_PXF);
			for(int y = y0; y >= y1 && m_velocity.y < 0.0f; --y)
			{
				for(int x = x0; x <= x1 && m_velocity.y < 0.0f; ++x)
				{
					if(m_terrain->isBlockAt(x, y, WORLD_LAYER_MIDDLE))
					{
						m_position.y = y * BLOCK_PXF + BLOCK_PXF;
						m_velocity.y = 0.0f;
						m_contact |= NORTH;
					}
				}
			}
		}

		// Apply horizontal velocity
		m_position.x += m_velocity.x;
		
		// Check collision horizontally
		if(m_velocity.x > 0.0f)
		{
			int y0 = (int)floor(m_position.y/BLOCK_PXF), y1 = (int)floor((m_position.y+m_size.y-1)/BLOCK_PXF);
			int x0 = (int)floor((m_position.x+m_size.x-1)/BLOCK_PXF), x1 = (int)floor((m_position.x+m_size.x-1+m_velocity.x)/BLOCK_PXF);
			for(int x = x0; x <= x1 && m_velocity.x > 0.0f; ++x)
			{
				for(int y = y0; y <= y1 && m_velocity.x > 0.0f; ++y)
				{
					if(m_terrain->isBlockAt(x, y, WORLD_LAYER_MIDDLE))
					{
						m_position.x = x * BLOCK_PXF - m_size.x;
						m_velocity.x = 0.0f;
						m_contact |= EAST;
					}
				}
			}
		}
		else if(m_velocity.x < 0.0f)
		{
			int y0 = (int)floor(m_position.y/BLOCK_PXF), y1 = (int)floor((m_position.y+m_size.y-1)/BLOCK_PXF);
			int x0 = (int)floor(m_position.x/BLOCK_PXF), x1 = (int)floor((m_position.x+m_velocity.x)/BLOCK_PXF);
			for(int x = x0; x >= x1 && m_velocity.x < 0.0f; --x)
			{
				for(int y = y0; y <= y1 && m_velocity.x < 0.0f; ++y)
				{
					if(m_terrain->isBlockAt(x, y, WORLD_LAYER_MIDDLE))
					{
						m_position.x = x * BLOCK_PXF + BLOCK_PXF;
						m_velocity.x = 0.0f;
						m_contact |= WEST;
					}
				}
			}
		}
	}
}