#include "Torch.h"

Torch::Torch(World *world, int x, int y, const BlockEntityData *data) :
	BlockEntity(world, x, y, data),
	m_pointlight(world->getLighting(), Vector2F(x, y) + Vector2F(0.5f, 0.5f), 10.0f, Color(255, 190, 90, 255))
{
	// updateFrame();
}

void Torch::onNeighbourChanged(NeighborChangedEvent *e)
{
	int frame = 0;
	if(!m_world->getTerrain()->isBlockAt(getX(), getY() + 1, WORLD_LAYER_MIDDLE) && !m_world->getTerrain()->isBlockAt(getX(), getY(), WORLD_LAYER_BACK))
	{
		if(m_world->getTerrain()->isBlockAt(getX() + 1, getY(), WORLD_LAYER_MIDDLE))
		{
			frame = 2;
		}
		else if(m_world->getTerrain()->isBlockAt(getX() - 1, getY(), WORLD_LAYER_MIDDLE))
		{
			frame = 1;
		}
		else
		{
			m_world->getTerrain()->removeBlockEntityAt(getX(), getY(), WORLD_LAYER_MIDDLE, true);
			return;
		}
	}
	m_world->getTerrain()->setBlockEntityFrameAt(getX(), getY(), WORLD_LAYER_MIDDLE, frame);
}