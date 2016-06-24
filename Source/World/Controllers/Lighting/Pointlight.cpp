#include "Pointlight.h"
#include "World/World.h"

Vertex *Pointlight::s_vertices;

Pointlight::Pointlight(World *world, Type type, const Vector2F &position, const float radius, const Color &color) :
	LightSource(world, type, position, radius, color)
{
	/*int x0 = math::floor((position.x - radius) / CHUNK_BLOCKSF), y0 = math::floor((position.y - radius) / CHUNK_BLOCKSF),
		x1 = math::floor((position.x + radius) / CHUNK_BLOCKSF), y1 = math::floor((position.y + radius) / CHUNK_BLOCKSF);
	for(int y = y0; y <= y1; y++)
	{
		for(int x = x0; x <= x1; x++)
		{
			Chunk *chunk = world->getTerrain()->getChunkManager()->getChunkAt(x, y, true);
			chunk->m_attached = false;
			//chunk->m_lightSources.push_back(this);
		}
	}*/
}