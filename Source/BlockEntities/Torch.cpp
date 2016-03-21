#include "Torch.h"

Torch::Torch(World *world, int x, int y, const BlockEntityData *data) :
	BlockEntity(world, x, y, data),
	m_time(0.0f),
	m_sprite(Game::GetInstance()->getResourceManager()->get<Texture2D>("Sprites/BlockEntities/LightSources/Torch"))//,
	//m_pointlight(world->getLighting(), Vector2((float) x, (float) y), 10.0f, Color(255, 190, 90))
{
	//m_terrain->setBlockTimeAt(torchPos.x, torchPos.y, torchPos.z, WORLD_LAYER_BACK);
	m_sprite.setPosition(x * BLOCK_PXF, y * BLOCK_PXF);
}

void Torch::onTick(TickEvent *e)
{
	// TODO: ChunkLoader::setFrameIndex() will set the frame index of the furniture
	// at the given position by updating a value in a texture object containing all
	// frame indexes
	//m_chunkLoader->setFrameIndex(m_position.x, m_position.y, m_time);
	m_time += e->getDelta();
}

void Torch::getVertices(vector<Vertex>& out)
{
}