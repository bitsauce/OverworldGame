#include "BlockEntity.h"
#include "World/World.h"

BlockEntity::BlockEntity(World *world, const int x, const int y, const BlockEntityData *data) :
	m_position(x, y),
	m_data(data),
	m_world(world)
{
}

BlockEntity::~BlockEntity()
{
}

void BlockEntity::getVertices(Vertex *vertices, uint *indices, int i)
{
	float x = getX() * BLOCK_PX, y = getY() * BLOCK_PXF;
	int cx = getX() - math::floor(getX() / CHUNK_BLOCKSF) * CHUNK_BLOCKSF, cy = getY() - math::floor(getY() / CHUNK_BLOCKSF) * CHUNK_BLOCKSF;

	vertices[0].set4f(VERTEX_POSITION, x, y);
	vertices[1].set4f(VERTEX_POSITION, x, y + BLOCK_PXF);
	vertices[2].set4f(VERTEX_POSITION, x + BLOCK_PXF, y);
	vertices[3].set4f(VERTEX_POSITION, x + BLOCK_PXF, y + BLOCK_PXF);

	TextureRegion region = BlockEntityData::s_atlas->get(getData()->getID(), 0,0, 1/3.0f, 1.0f);
	vertices[0].set4f(VERTEX_TEX_COORD, region.uv0.x, region.uv0.y);
	vertices[1].set4f(VERTEX_TEX_COORD, region.uv0.x, region.uv1.y);
	vertices[2].set4f(VERTEX_TEX_COORD, region.uv1.x, region.uv0.y);
	vertices[3].set4f(VERTEX_TEX_COORD, region.uv1.x, region.uv1.x);

	vertices[0].set4ub(VERTEX_COLOR, cx, cy, 255, 255);
	vertices[1].set4ub(VERTEX_COLOR, cx, cy, 255, 255);
	vertices[2].set4ub(VERTEX_COLOR, cx, cy, 255, 255);
	vertices[3].set4ub(VERTEX_COLOR, cx, cy, 255, 255);

	indices[0] = i * 4 + 0;
	indices[1] = i * 4 + 1;
	indices[2] = i * 4 + 2;
	indices[3] = i * 4 + 2;
	indices[4] = i * 4 + 1;
	indices[5] = i * 4 + 3;
}