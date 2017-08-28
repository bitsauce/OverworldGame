#include "Game/Game.h"
#include "BlockEntity.h"
#include "World/World.h"

BlockEntity::BlockEntity(const Json::Value &attributes) :
	m_position(attributes["x"].asInt(), attributes["y"].asInt()),
	m_data(reinterpret_cast<BlockEntityData*>(attributes["data_ptr"].asInt())),
	m_world(dynamic_cast<Overworld*>(Game::Get())->getClient()->getWorld())
{
}

BlockEntity::~BlockEntity()
{
}

void BlockEntity::onNeighbourChanged(NeighborChangedEvent *e)
{
	//LOG("Neighbor at [%i, %i] changed from %i to %i", e->getDx(), e->getDy(), e->getOldBlock()->getBlockID(), e->getNewBlock()->getBlockID());

	// If the placement is no longer valid, remove this block entity
	if(!m_data->isValidPlacement(m_position.x, m_position.y, m_world->getTerrain(), this))
	{
		m_world->getTerrain()->removeBlockEntityAt(m_position.x, m_position.y, m_data->getLayer(), true);
	}
}

void BlockEntity::getVertices(Vertex *vertices, uint *indices, int i)
{
	float x = getX() * BLOCK_PX, y = getY() * BLOCK_PXF;
	int w = getData()->getWidth(), h = getData()->getHeight();
	int cx = getX() - math::floor(getX() / CHUNK_BLOCKSF) * CHUNK_BLOCKSF, cy = getY() - math::floor(getY() / CHUNK_BLOCKSF) * CHUNK_BLOCKSF;

	vertices[0].set2f(VERTEX_POSITION, x, y);
	vertices[1].set2f(VERTEX_POSITION, x, y + BLOCK_PXF * h);
	vertices[2].set2f(VERTEX_POSITION, x + BLOCK_PXF * w, y);
	vertices[3].set2f(VERTEX_POSITION, x + BLOCK_PXF * w, y + BLOCK_PXF * h);

	vertices[0].set2f(VERTEX_TEX_COORD, 0.0f, 0.0f);
	vertices[1].set2f(VERTEX_TEX_COORD, 0.0f, 1.0f);
	vertices[2].set2f(VERTEX_TEX_COORD, 1.0f, 0.0f);
	vertices[3].set2f(VERTEX_TEX_COORD, 1.0f, 1.0f);

	vertices[0].set4ub(VERTEX_COLOR, cx, cy, getData()->getID(), 255);
	vertices[1].set4ub(VERTEX_COLOR, cx, cy, getData()->getID(), 255);
	vertices[2].set4ub(VERTEX_COLOR, cx, cy, getData()->getID(), 255);
	vertices[3].set4ub(VERTEX_COLOR, cx, cy, getData()->getID(), 255);

	indices[0] = i * 4 + 0;
	indices[1] = i * 4 + 1;
	indices[2] = i * 4 + 2;
	indices[3] = i * 4 + 2;
	indices[4] = i * 4 + 1;
	indices[5] = i * 4 + 3;
}