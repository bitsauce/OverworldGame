#include "physicsbody.h"
#include "constants.h"

#include "game/world.h"
#include "terrain/terrain.h"

#define PHYSICS_GRAVITY 0.5f

PhysicsBody::PhysicsBody() :
	m_velocity(0.0f, 0.0f),
	m_position(0.0f, 0.0f),
	m_rotation(45.0f),
	m_gravityScale(1.0f),
	m_allowRotation(false),
	m_terrain(World::getTerrain())
{
}

void PhysicsBody::update()
{
	m_velocity.y += PHYSICS_GRAVITY * m_gravityScale;

	if(m_allowRotation)
	{
		/*Vector2 points[4];
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
					if(m_terrain->getBlockAt(x0, y, TERRAIN_LAYER_SCENE) > BLOCK_OCCUPIED)
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
		m_position.y += m_velocity.y;

		int x0 = floor(m_position.x/BLOCK_PXF);
		int y0 = floor(m_position.y/BLOCK_PXF);
		int x1 = floor((m_position.x+m_size.x-1)/BLOCK_PXF);
		int y1 = floor((m_position.y+m_size.y-1)/BLOCK_PXF);
		int vx = floor((m_position.x+m_size.x-1+m_velocity.x)/BLOCK_PXF);
		int vy = floor((m_position.y+m_size.y-1+m_velocity.y)/BLOCK_PXF);
	
		// Check down
		if(m_velocity.y > 0.0f)
		{
			for(int y = y1; y <= vy && m_velocity.y > 0.0f; ++y)
			{
				for(int x = x0; x <= x1 && m_velocity.y > 0.0f; ++x)
				{
					if(m_terrain->getBlockAt(x, y, TERRAIN_LAYER_SCENE) > BLOCK_OCCUPIED)
					{
						m_position.y = y * BLOCK_PXF - m_size.y;
						m_velocity.y = 0.0f;

						y0 = floor(m_position.y/BLOCK_PXF);
						y1 = floor((m_position.y+m_size.y-1)/BLOCK_PXF);
						vy = 0;
					}
				}
			}
		}
		else if(m_velocity.y < 0.0f)
		{

		}

		m_position.x += m_velocity.x;
		x0 = floor(m_position.x/BLOCK_PXF);
		x1 = floor((m_position.x+m_size.x-1)/BLOCK_PXF);
		vx = floor((m_position.x+m_size.x-1+m_velocity.x)/BLOCK_PXF);
		
		// Check right
		if(m_velocity.x > 0.0f)
		{
			for(int x = x1; x <= vx && m_velocity.x > 0.0f; ++x)
			{
				for(int y = y0; y <= y1 && m_velocity.x > 0.0f; ++y)
				{
					if(m_terrain->getBlockAt(x, y, TERRAIN_LAYER_SCENE) > BLOCK_OCCUPIED)
					{
						m_position.x = x * BLOCK_PXF - m_size.x;
						m_velocity.x = 0.0f;
					}
				}
			}
		}
		else if(m_velocity.x < 0.0f)
		{
		}
	}
}

void PhysicsBody::draw(XBatch *batch)
{
}