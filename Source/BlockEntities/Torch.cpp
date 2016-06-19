#include "Torch.h"

Torch::Torch(World *world, int x, int y, const BlockEntityData *data) :
	BlockEntity(world, x, y, data),
	m_pointlight(world, Vector2F(x, y) + Vector2F(0.5f, 0.5f), 10.0f, Color(255, 190, 90, 255))
{
	updateBaseFrame();
}

void Torch::updateBaseFrame()
{
	// Sets the correct torch base frame
	uint frame = 0;
	Terrain *terrain = m_world->getTerrain();
	if(!terrain->isBlockAt(getX(), getY() + 1, WORLD_LAYER_MIDDLE) && !terrain->isBlockAt(getX(), getY(), WORLD_LAYER_BACK))
	{
		if(terrain->isBlockAt(getX() + 1, getY(), WORLD_LAYER_MIDDLE))
		{
			frame = 2;
		}
		else if(terrain->isBlockAt(getX() - 1, getY(), WORLD_LAYER_MIDDLE))
		{
			frame = 1;
		}
	}
	terrain->setBlockEntityFrameAt(getX(), getY(), WORLD_LAYER_MIDDLE, frame);
}

void Torch::onNeighbourChanged(NeighborChangedEvent *e)
{
	updateBaseFrame();
	BlockEntity::onNeighbourChanged(e);
}