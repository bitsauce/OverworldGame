#include "BitStream.h"
#include "Networking/Server.h"
#include "Networking/Client.h"

#include "Terrain.h"
#include "Constants.h"
#include "Blocks/BlockData.h"

#include "World/World.h"

#include "Entities/ItemDrop.h"

#include "Entities/EntityData.h"
#include "BlockEntities/BlockEntity.h"
#include "BlockEntities/BlockEntityData.h"

Terrain::Terrain(World *world, Window *window) :
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
bool Terrain::setBlockAt(const int x, const int y, const Block block, const WorldLayer layer = WORLD_LAYER_MIDDLE)
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

	if(block == BLOCK_ENTITY)
	{
		BlockEntityData *data = (BlockEntityData*) block.getBlockData();
		if(!data)
		{
			return false;
		}

		for(int y1 = y; y1 < y + data->getHeight(); y1++)
		{
			for(int x1 = x; x1 < x + data->getWidth(); x1++)
			{
				m_chunkManager->getChunkAt((int) floor(x / CHUNK_BLOCKSF), (int) floor(y / CHUNK_BLOCKSF)).setBlockAt(math::mod(x, CHUNK_BLOCKS), math::mod(y, CHUNK_BLOCKS), block, layer);
			}
		}
		return m_chunkManager->getChunkAt((int) floor(x / CHUNK_BLOCKSF), (int) floor(y / CHUNK_BLOCKSF)).addBlockEntity(math::mod(x, CHUNK_BLOCKS), math::mod(y, CHUNK_BLOCKS), block, layer);
	}

	return m_chunkManager->getChunkAt((int)floor(x / CHUNK_BLOCKSF), (int)floor(y / CHUNK_BLOCKSF)).setBlockAt(math::mod(x, CHUNK_BLOCKS), math::mod(y, CHUNK_BLOCKS), block, layer);
}

Block Terrain::getBlockAt(const int x, const int y, const WorldLayer layer = WORLD_LAYER_MIDDLE)
{
	return m_chunkManager->getChunkAt((int)floor(x / CHUNK_BLOCKSF), (int)floor(y / CHUNK_BLOCKSF)).getBlockAt(math::mod(x, CHUNK_BLOCKS), math::mod(y, CHUNK_BLOCKS), layer);
}

bool Terrain::isBlockAt(const int x, const int y, const WorldLayer layer = WORLD_LAYER_MIDDLE)
{
	return getBlockAt(x, y, layer) != BLOCK_EMPTY;
}

bool Terrain::removeBlockAt(const int x, const int y, const WorldLayer layer = WORLD_LAYER_MIDDLE)
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

/*void Terrain::setBlockEntityAt(const int x, const int y, const BlockEntityID blockEntityID)
{
	BlockEntityData *data = BlockEntityData::get(blockEntityID);
	if(data)
	{
		data->create(m_game->getWorld(), blockPos.x, blockPos.y);
		pawn->getCurrentItem()->dec();
	}

	for(int y1 = y; y1 < y + data->getHeight(); y1++)
	{
		for(int x1 = x; x1 < x + data->getWidth(); x1++)
		{
			m_chunkManager->getChunkAt((int) floor(x / CHUNK_BLOCKSF), (int) floor(y / CHUNK_BLOCKSF)).setBlockAt(math::mod(x, CHUNK_BLOCKS), math::mod(y, CHUNK_BLOCKS), BLOCK_ENTITY, WORLD_LAYER_MIDDLE);
		}
	}
}*/