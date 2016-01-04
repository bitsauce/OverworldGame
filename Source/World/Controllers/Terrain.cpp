#include "BitStream.h"
#include "Networking/Server.h"
#include "Networking/Client.h"

#include "Terrain.h"
#include "Constants.h"
#include "Blocks/BlockData.h"

#include "World/World.h"

#include "Entities/Dynamic/ItemDrop.h"

#include "Entities/EntityData.h"
#include "BlockEntities/BlockEntity.h"
#include "BlockEntities/BlockEntityData.h"

Terrain::Terrain(World *world, Window *window) :
	Entity(world, ENTITY_TERRAIN),
	m_world(world)
{
	m_chunkManager = new ChunkManager(world, window);
	addChildLast(m_chunkManager);
}

Terrain::~Terrain()
{
	delete m_chunkManager;
}

// BLOCKS
bool Terrain::setBlockAt(const int x, const int y, BlockID block, const WorldLayer layer = WORLD_LAYER_MIDDLE)
{
	/*if(Connection::getInstance()->isServer())
	{
		RakNet::BitStream bitStream;
		bitStream.Write((RakNet::MessageID)ID_SET_BLOCK);
		bitStream.Write(x);
		bitStream.Write(y);
		bitStream.Write(block);
		bitStream.Write(layer);
		((Server*)Connection::getInstance())->sendPacket(&bitStream);
	}
	else if(Client::getInstance())
	{
		RakNet::BitStream bitStream;
		bitStream.Write((RakNet::MessageID)ID_SET_BLOCK);
		bitStream.Write(x);
		bitStream.Write(y);
		bitStream.Write(block);
		bitStream.Write(layer);
		((Client*)Connection::getInstance())->sendPacket(&bitStream);
	}*/

	return m_chunkManager->getChunkAt((int)floor(x / CHUNK_BLOCKSF), (int)floor(y / CHUNK_BLOCKSF)).setBlockAt(math::mod(x, CHUNK_BLOCKS), math::mod(y, CHUNK_BLOCKS), block, layer);
}

BlockID Terrain::getBlockAt(const int x, const int y, const WorldLayer layer = WORLD_LAYER_MIDDLE)
{
	return m_chunkManager->getChunkAt((int)floor(x / CHUNK_BLOCKSF), (int)floor(y / CHUNK_BLOCKSF)).getBlockAt(math::mod(x, CHUNK_BLOCKS), math::mod(y, CHUNK_BLOCKS), layer);
}

bool Terrain::isBlockAt(const int x, const int y, WorldLayer layer = WORLD_LAYER_MIDDLE)
{
	return getBlockAt(x, y, layer) != BLOCK_EMPTY;
}

bool Terrain::removeBlockAt(const int x, const int y, WorldLayer layer = WORLD_LAYER_MIDDLE)
{
	BlockID blockID = getBlockAt(x, y, layer);
	if(setBlockAt(x, y, BLOCK_EMPTY, layer))
	{
		//ItemDrop *itemDrop = new ItemDrop(m_world, BlockData::get(blockID).getItem());
		//itemDrop->setPosition(x * BLOCK_PXF, y * BLOCK_PXF);
		return true;
	}
	return false;
}

void Terrain::placeStaticEntity(BlockEntity *entity)
{
	Vector2i pos = entity->getPosition();
	m_chunkManager->getChunkAt((int) floor(pos.x / CHUNK_BLOCKSF), (int) floor(pos.y / CHUNK_BLOCKSF)).addStaticEntity(entity);
	for(int y = pos.y; y < pos.y + (int) entity->getData()->getHeight(); ++y)
	{
		for(int x = pos.x; x < pos.x + (int) entity->getData()->getWidth(); ++x)
		{
			m_chunkManager->getChunkAt((int) floor(x / CHUNK_BLOCKSF), (int) floor(y / CHUNK_BLOCKSF)).setBlockAt(math::mod(x, CHUNK_BLOCKS), math::mod(y, CHUNK_BLOCKS), BLOCK_ENTITY, WORLD_LAYER_MIDDLE);
		}
	}
}