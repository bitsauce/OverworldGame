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
	Vertex vertices[4];

	float x = getX() * BLOCK_PX, y = getY() * BLOCK_PXF;

	vertices[0].set4f(VERTEX_POSITION, x, y);
	vertices[1].set4f(VERTEX_POSITION, x, y + BLOCK_PXF);
	vertices[2].set4f(VERTEX_POSITION, x + BLOCK_PXF, y);
	vertices[3].set4f(VERTEX_POSITION, x + BLOCK_PXF, y + BLOCK_PXF);

	TextureRegion region = BlockEntityData::s_atlas->get(getData()->getID());
	vertices[0].set4f(VERTEX_TEX_COORD, region.uv0.x, region.uv0.y);
	vertices[1].set4f(VERTEX_TEX_COORD, region.uv0.x, region.uv1.y);
	vertices[2].set4f(VERTEX_TEX_COORD, region.uv1.x, region.uv0.y);
	vertices[3].set4f(VERTEX_TEX_COORD, region.uv1.x, region.uv1.x);

	vertices[0].set4ub(VERTEX_COLOR, 255, 255, 255, 255);
	vertices[1].set4ub(VERTEX_COLOR, 255, 255, 255, 255);
	vertices[2].set4ub(VERTEX_COLOR, 255, 255, 255, 255);
	vertices[3].set4ub(VERTEX_COLOR, 255, 255, 255, 255);

	out.push_back(vertices[0]);
	out.push_back(vertices[1]);
	out.push_back(vertices[2]);

	out.push_back(vertices[2]);
	out.push_back(vertices[1]);
	out.push_back(vertices[3]);
}