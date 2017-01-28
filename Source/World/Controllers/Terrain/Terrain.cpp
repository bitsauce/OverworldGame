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
bool Terrain::setBlockAt(const int x, const int y, const WorldLayer layer, const BlockData *blockData, const uint subID, const bool replace)
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

	Chunk *chunk = m_chunkManager->getChunkAt((int) floor(x / CHUNK_BLOCKSF), (int) floor(y / CHUNK_BLOCKSF), true);
	/*const */ChunkBlock &block = chunk->getChunkBlockAt(math::mod(x, CHUNK_BLOCKS), math::mod(y, CHUNK_BLOCKS), layer);

	if(!replace)
	{
		// Check if we can place a block here
		if(blockData->getID() != 0 || (layer == WORLD_LAYER_MIDDLE && block.getBlockEntity()))
		{
			return false;
		}
	}

	block.setBlockSubID(subID);

	return chunk->setBlockAt(math::mod(x, CHUNK_BLOCKS), math::mod(y, CHUNK_BLOCKS), layer, blockData);
}

const BlockData *Terrain::getBlockAt(const int x, const int y, const WorldLayer layer)
{
	return m_chunkManager->getChunkAt((int)floor(x / CHUNK_BLOCKSF), (int)floor(y / CHUNK_BLOCKSF), true)->getBlockAt(math::mod(x, CHUNK_BLOCKS), math::mod(y, CHUNK_BLOCKS), layer);
}

bool Terrain::isBlockAt(const int x, const int y, const WorldLayer layer)
{
	return getBlockAt(x, y, layer)->isSolid();
}

#include "Items/ItemData.h"

bool Terrain::removeBlockAt(const int x, const int y, const WorldLayer layer, const bool createItem)
{
	const BlockData *block = getBlockAt(x, y, layer);
	if(setBlockAt(x, y, layer, BlockData::get(0),  0, true))
	{
		if(createItem)
		{
			ItemDrop *itemDrop = new ItemDrop(m_world, block->getItem()->getID());
			itemDrop->setPosition(x * BLOCK_PXF, y * BLOCK_PXF);
		}
		return true;
	}
	return false;
}

BlockEntity *Terrain::createBlockEntityAt(const int x, const int y, const BlockEntityID blockEntityID, const bool replace)
{
	BlockEntityData *data = BlockEntityData::get(blockEntityID);
	if(!data) return 0;

	if(replace)
	{
		// Make sure block and block entites are removed
		for(int y1 = y; y1 < y + data->getHeight(); y1++)
		{
			for(int x1 = x; x1 < x + data->getWidth(); x1++)
			{
				Chunk *chunk = m_chunkManager->getChunkAt((int) floor(x1 / CHUNK_BLOCKSF), (int) floor(y1 / CHUNK_BLOCKSF), true);

				BlockEntity *blockEntity = chunk->getBlockEntityAt(math::mod(x1, CHUNK_BLOCKS), math::mod(y1, CHUNK_BLOCKS), data->getLayer());
				if(blockEntity)
				{
					m_chunkManager->getChunkAt((int) floor(blockEntity->getX() / CHUNK_BLOCKSF), (int) floor(blockEntity->getY() / CHUNK_BLOCKSF), true)->removeBlockEntity(blockEntity);
				}

				if(data->getLayer() == WORLD_LAYER_MIDDLE)
				{
					chunk->setBlockAt(math::mod(x1, CHUNK_BLOCKS), math::mod(y1, CHUNK_BLOCKS), data->getLayer(), 0);
				}
			}
		}
	}

	// Check if we can place the block entity here
	if(!data->isValidPlacement(x, y, this, 0))
	{
		return 0;
	}

	BlockEntity *blockEntity = data->create(m_world, x, y);

	// Set block entity in all the positions it occupies
	for(int y1 = y; y1 < y + data->getHeight(); y1++)
	{
		for(int x1 = x; x1 < x + data->getWidth(); x1++)
		{
			m_chunkManager->getChunkAt((int) floor(x1 / CHUNK_BLOCKSF), (int) floor(y1 / CHUNK_BLOCKSF), true)->setBlockEntityAt(math::mod(x1, CHUNK_BLOCKS), math::mod(y1, CHUNK_BLOCKS), data->getLayer(), blockEntity);
		}
	}
	m_chunkManager->getChunkAt((int) floor(x / CHUNK_BLOCKSF), (int) floor(y / CHUNK_BLOCKSF), true)->addBlockEntity(blockEntity);
	return blockEntity;
}

BlockEntity *Terrain::getBlockEntityAt(const int x, const int y, const WorldLayer layer)
{
	return m_chunkManager->getChunkAt((int) floor(x / CHUNK_BLOCKSF), (int) floor(y / CHUNK_BLOCKSF), true)->getBlockEntityAt(math::mod(x, CHUNK_BLOCKS), math::mod(y, CHUNK_BLOCKS), layer);
}

bool Terrain::isBlockEntityAt(const int x, const int y, const WorldLayer layer)
{
	return getBlockEntityAt(x, y ,layer) != 0;
}

bool Terrain::removeBlockEntityAt(const int x, const int y, const WorldLayer layer, const bool createItem)
{
	BlockEntity *blockEntity = getBlockEntityAt(x, y, layer);
	if(blockEntity)
	{
		m_chunkManager->getChunkAt((int) floor(blockEntity->getX() / CHUNK_BLOCKSF), (int) floor(blockEntity->getY() / CHUNK_BLOCKSF), true)->removeBlockEntity(blockEntity);
		if(createItem)
		{
			// TODO: Create item
		}
		return true;
	}
	return false;
}

bool Terrain::setBlockEntityFrameAt(const int x, const int y, const WorldLayer layer, const uint frame)
{
	return m_chunkManager->getChunkAt((int) floor(x / CHUNK_BLOCKSF), (int) floor(y / CHUNK_BLOCKSF), true)->setBlockEntityFrameAt(math::mod(x, CHUNK_BLOCKS), math::mod(y, CHUNK_BLOCKS), layer, frame);
}

bool Terrain::isEmptyAt(const int x, const int y, const WorldLayer layer)
{
	return m_chunkManager->getChunkAt((int) floor(x / CHUNK_BLOCKSF), (int) floor(y / CHUNK_BLOCKSF), true)->isEmptyAt(math::mod(x, CHUNK_BLOCKS), math::mod(y, CHUNK_BLOCKS), layer);
}