#include "Torch.h"

Torch::Torch(const Json::Value &attributes) :
	BlockEntity(attributes),
	m_pointlight(dynamic_cast<Overworld*>(Game::Get())->getClient()->getWorld())
{
	m_pointlight.setMobility(LightSource::STATIC);
	m_pointlight.setPosition(getPosition() + Vector2F(0.5f, 0.5f));
	m_pointlight.setRadius(10.0f);
	m_pointlight.setColor(Color(255, 190, 90, 255));

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
	terrain->setBlockEntityUVAt(getX(), getY(), WORLD_LAYER_MIDDLE, Vector2F(frame * m_data->getFrameColumns() * BLOCK_PXF, 0.0f));
}

void Torch::onNeighbourChanged(NeighborChangedEvent *e)
{
	updateBaseFrame();
	BlockEntity::onNeighbourChanged(e);
}