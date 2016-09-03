#include "Spotlight.h"
#include "World/World.h"

Spotlight::Spotlight(World *world) :
	LightSource(world, SPOTLIGHT),
	m_coneAngle(30.0f)
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

void Spotlight::setConeAngle(const float f)
{
	m_coneAngle = f;
	onModified();
}

float Spotlight::getConeAngle() const
{
	return m_coneAngle;
}
