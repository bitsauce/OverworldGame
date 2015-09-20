#include "Torch.h"
#include "ThingData.h"
#include "Constants.h"
#include "World/World.h"
#include "World/Terrain/Terrain.h"
#include "World/Terrain/ChunkLoader.h"
#include "Entities/ItemDrop.h"

Torch::Torch(World *world, const int x, const int y) :
	Thing(x, y),
	m_world(world),
	m_sprite(ThingData::get(BLOCK_ENTITY_TORCH).getSprite()),
	m_spotlight(world->getLighting(), Vector2(x, y), 10.0f, Color(255, 190, 90))
{
	m_sprite.setPosition(x * BLOCK_PXF, y * BLOCK_PXF);
}

void Torch::activate()
{
	int x = (int)floor(getX() / CHUNK_BLOCKSF), y = (int)floor(getY() / CHUNK_BLOCKSF);

	new ItemDrop(m_world, m_sprite.getPosition(), ITEM_TORCH);
	m_world->getTerrain()->getChunkLoader()->getChunkAt(x, y).removeThing(this);
	m_world->getTerrain()->setBlockAt(getX(), getY(), BLOCK_EMPTY, TERRAIN_LAYER_MIDDLE);

	//delete this;
}

void Torch::draw(SpriteBatch *spriteBatch, const float alpha)
{
	spriteBatch->drawSprite(m_sprite);
}