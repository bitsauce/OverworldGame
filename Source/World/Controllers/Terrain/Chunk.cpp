#include "Chunk.h"
#include "ChunkManager.h"
#include "Constants.h"
#include "World/World.h"
#include "Generation/Generator.h"

const uint OPPOSITE_INDEX[8] = { 4, 5, 6, 7, 0, 1, 2, 3 };
const int DIR_INDEX[16] = { 0, 5, 1, -1, 3, 4, 2, -1, 7, 6, 0, -1, -1, -1, -1, -1 };
const int DIR_X[8] = { -1,  0,  1,  1,  1,  0, -1, -1 };
const int DIR_Y[8] = { -1, -1, -1,  0,  1,  1,  1,  0 };

Chunk::Chunk(ChunkManager *chunkManager) :
	m_chunkManager(chunkManager),
	m_generateBlockEntityBuffers(false)
{
	// Setup flags and such
	m_attached = m_sorted = m_modified = false; // Not modified

	// Initialize blocks
	m_blocks = new BlockID[CHUNK_BLOCKS * CHUNK_BLOCKS * WORLD_LAYER_COUNT];
	m_blockEntities = new BlockEntity*[CHUNK_BLOCKS * CHUNK_BLOCKS * WORLD_LAYER_COUNT];
	m_blockTexture = shared_ptr<Texture2D>(new Texture2D(CHUNK_BLOCKS, CHUNK_BLOCKS));

	uchar data[4] = { 0 };
	Random rand;
	Pixmap pixmap(CHUNK_BLOCKS, CHUNK_BLOCKS);
	for(int y = 0; y < CHUNK_BLOCKS; y++)
	{
		for(int x = 0; x < CHUNK_BLOCKS; x++)
		{
			data[0] = 0;//rand.nextInt(255);
			pixmap.setPixel(x, y, &data);
		}
	}
	m_blockEntitiesOffsetUVsTexture = shared_ptr<Texture2D>(new Texture2D(pixmap));
}

void Chunk::load(int chunkX, int chunkY, BlockID *blocks)
{
	// Set position
	m_x = chunkX;
	m_y = chunkY;

	// Set all blocks
	for (int z = 0; z < WORLD_LAYER_COUNT; ++z)
	{
		for (int y = 0; y < CHUNK_BLOCKS; ++y)
		{
			for (int x = 0; x < CHUNK_BLOCKS; ++x)
			{
				m_blocks[BLOCK_INDEX(x, y, z)] = blocks[BLOCK_INDEX(x, y, z)];
				m_blockEntities[BLOCK_INDEX(x, y, z)] = 0;
			}
		}
	}

	// Load block and shadow textures
	Pixmap pixmap(CHUNK_BLOCKS, CHUNK_BLOCKS);
	uchar pixel[4];
	for(int y = 0; y < CHUNK_BLOCKS; y++)
	{
		for(int x = 0; x < CHUNK_BLOCKS; x++)
		{
			float shadow = 1.0f;
			for(int z = 0; z < WORLD_LAYER_COUNT; ++z)
			{
				const BlockData *data = BlockData::get(m_blocks[BLOCK_INDEX(x, y, z)]);
				pixel[z] = (uchar) data->getID();
				shadow -= data->getOpacity();
			}
			pixel[3] = uchar(255 * max(shadow, 0.0f));
			pixmap.setPixel(x, y, pixel);
		}
	}

	// Setup neighbour chunks
	for(int i = 0; i < 8; i++)
	{
		// Get neighbour chunk without generating it
		// If it exists we update the neighbourhood relationship for both chunks
		Chunk *neighbourChunk = m_chunkManager->getChunkAt(m_x + DIR_X[i], m_y + DIR_Y[i], false);
		m_neighborChunks[i] = neighbourChunk;
		if(neighbourChunk)
		{
			neighbourChunk->m_neighborChunks[OPPOSITE_INDEX[i]] = this;
		}
	}

	// Create tile map texture
	m_blockTexture->updatePixmap(pixmap);

	// Mark as not modified
	m_attached = m_sorted = m_modified = false;

	// Generate block entity buffers
	m_generateBlockEntityBuffers = true;
}

void Chunk::unload()
{
	// Update neighbour chunks
	for(int i = 0; i < 8; i++)
	{
		Chunk *neighbourChunk = m_chunkManager->getChunkAt(m_x + DIR_X[i], m_y + DIR_Y[i], false);
		if(neighbourChunk)
		{
			neighbourChunk->m_neighborChunks[OPPOSITE_INDEX[i]] = 0;
		}
	}

	// Remove block entities
	for(BlockEntity *entity : list<BlockEntity*>(m_blockEntityList))
	{
		removeBlockEntity(entity);
	}
}

bool Chunk::setBlockAt(const int x, const int y, const WorldLayer layer, const BlockID newBlock)
{
	// Check if we need to do anything
	const BlockID block = m_blocks[BLOCK_INDEX(x, y, layer)];
	if(block != newBlock)
	{
		if(m_neighborChunks[0] && x == 0 && y == 0)                               { m_neighborChunks[0]->m_sorted = m_neighborChunks[0]->m_attached = false; }
		if(m_neighborChunks[1] && y == 0)                                         { m_neighborChunks[1]->m_sorted = m_neighborChunks[1]->m_attached = false; }
		if(m_neighborChunks[2] && x == CHUNK_BLOCKS - 1 && y == 0)                { m_neighborChunks[2]->m_sorted = m_neighborChunks[2]->m_attached = false; }
		if(m_neighborChunks[3] && x == CHUNK_BLOCKS - 1)                          { m_neighborChunks[3]->m_sorted = m_neighborChunks[3]->m_attached = false; }
		if(m_neighborChunks[4] && x == CHUNK_BLOCKS - 1 && y == CHUNK_BLOCKS - 1) { m_neighborChunks[4]->m_sorted = m_neighborChunks[4]->m_attached = false; }
		if(m_neighborChunks[5] && y == CHUNK_BLOCKS - 1)                          { m_neighborChunks[5]->m_sorted = m_neighborChunks[5]->m_attached = false; }
		if(m_neighborChunks[6] && x == 0 && y == CHUNK_BLOCKS - 1)                { m_neighborChunks[6]->m_sorted = m_neighborChunks[6]->m_attached = false; }
		if(m_neighborChunks[7] && x == 0)                                         { m_neighborChunks[7]->m_sorted = m_neighborChunks[7]->m_attached = false; }

		// Set the block value
		m_blocks[BLOCK_INDEX(x, y, layer)] = newBlock;
		m_sorted = m_attached = false; m_modified = true; // Mark chunk as modified
		
		// Get neighboring block entities
		set<BlockEntity*> neighbourBlockEntities;
		for(int i = 0; i < 8; i++)
		{
			const tuple<BlockID, BlockEntity*> neighborBlock = getNeighborBlock(x + DIR_X[i], y + DIR_Y[i], layer);
			if(get<1>(neighborBlock) != nullptr)
			{
				neighbourBlockEntities.insert(get<1>(neighborBlock));
			}
		}

		{
			// This ensures that block entities attached to the background will be destroyed when background is removed
			// ... This is not the best solution
			BlockEntity *blockEntity = m_blockEntities[BLOCK_INDEX(x, y, WORLD_LAYER_MIDDLE)];
			if(blockEntity)
			{
				neighbourBlockEntities.insert(blockEntity);
			}
		}
		
		// Notify block entities
		for(BlockEntity *blockEntity : neighbourBlockEntities)
		{
			NeighborChangedEvent e((x + m_x * CHUNK_BLOCKS) - blockEntity->getX(), (y + m_y * CHUNK_BLOCKS) - blockEntity->getY(), block, m_blocks[BLOCK_INDEX(x, y, layer)]);
			blockEntity->onNeighbourChanged(&e);
		}

		// Update block texture
		uchar pixel[4];
		float shadow = 1.0f;
		for(int z = 0; z < WORLD_LAYER_COUNT; ++z)
		{
			const BlockData *data = BlockData::get(m_blocks[BLOCK_INDEX(x, y, z)]);
			pixel[z] = (uchar) data->getID();
			shadow -= data->getOpacity();
		}
		pixel[3] = (uchar) (255 * max(shadow, 0.0f));
		m_blockTexture->updatePixmap(x, y, Pixmap(1, 1, pixel));

		return true; // Return true as something was changed
	}
	return false; // Nothing changed
}

const BlockData *Chunk::getBlockDataAt(const int x, const int y, const WorldLayer layer) const
{
	return BlockData::get(m_blocks[BLOCK_INDEX(x, y, layer)]);
}

BlockID Chunk::getBlockAt(const int x, const int y, const WorldLayer layer) const
{
	return m_blocks[BLOCK_INDEX(x, y, layer)];
}

bool Chunk::isEmptyAt(const int x, const int y, const WorldLayer layer) const
{
	return m_blocks[BLOCK_INDEX(x, y, layer)] == 0;
}

void Chunk::addBlockEntity(BlockEntity *blockEntity)
{
	// Add to block entity list
	m_blockEntityList.push_back(blockEntity);
	m_generateBlockEntityBuffers = true;
}

bool Chunk::removeBlockEntity(BlockEntity *blockEntity)
{
	BlockEntityData *data = blockEntity->getData();

	// Remove block entity from all the positions it occupied
	for(int y = blockEntity->getY(); y < data->getHeight() + blockEntity->getY(); y++)
	{
		for(int x = blockEntity->getX(); x < data->getWidth() + blockEntity->getX(); x++)
		{
			m_chunkManager->getChunkAt((int) floor(x / CHUNK_BLOCKSF), (int) floor(y / CHUNK_BLOCKSF), true)->m_blockEntities[BLOCK_INDEX(math::mod(x, CHUNK_BLOCKS), math::mod(y, CHUNK_BLOCKS), data->getLayer())] = 0;
		}
	}

	// Remove block entity from list
	m_generateBlockEntityBuffers = true;
	m_blockEntityList.remove(blockEntity);

	// Delete object
	delete blockEntity;
	return true;
}

void Chunk::setBlockEntityAt(const int x, const int y, const WorldLayer layer, BlockEntity *blockEntity)
{
	m_blockEntities[BLOCK_INDEX(x, y, layer)] = blockEntity;
}

BlockEntity *Chunk::getBlockEntityAt(const int x, const int y, const WorldLayer layer) const
{
	return m_blockEntities[BLOCK_INDEX(x, y, layer)];
}

bool Chunk::setBlockEntityUVAt(const int x, const int y, const WorldLayer layer, const Vector2I &uvOffset)
{
	uchar data[4];
	data[0] = uchar(uvOffset.x & 0xFF);
	data[1] = uchar((uvOffset.x >> 8) & 0xFF);
	data[2] = uchar(uvOffset.y & 0xFF);
	data[3] = uchar((uvOffset.y >> 8) & 0xFF);
	m_blockEntitiesOffsetUVsTexture->updatePixmap(x, y, Pixmap(1, 1, data));
	return true;
}

void Chunk::attach(GraphicsContext *context, const int x, const int y)
{
	// Draw blocks
	context->setTexture(m_blockTexture);
	context->drawRectangle(x * CHUNK_BLOCKS, y * CHUNK_BLOCKS, CHUNK_BLOCKS, CHUNK_BLOCKS);
	context->setTexture(0);

	m_attached = true;
}

void Chunk::detach()
{
	m_attached = m_sorted = false;
}

void Chunk::drawBlockEntities(GraphicsContext *context)
{
	if(m_generateBlockEntityBuffers)
	{
		Vertex *vertices = new Vertex[m_blockEntityList.size() * 4];
		uint *indices = new uint[m_blockEntityList.size() * 6];
		int i = 0;
		for(BlockEntity *blockEntity : m_blockEntityList)
		{
			blockEntity->getVertices(vertices + i * 4, indices + i * 6, i);
			i++;
		}

		m_blockEntityVBO.setData(vertices, m_blockEntityList.size() * 4);
		m_blockEntityIBO.setData(indices, m_blockEntityList.size() * 6);
		delete[] vertices;

		m_generateBlockEntityBuffers = false;
	}

	// TODO: I should store BlockEntities in a global buffer of some sort.
	context->drawIndexedPrimitives(GraphicsContext::PRIMITIVE_TRIANGLES, &m_blockEntityVBO, &m_blockEntityIBO);
}

tuple<BlockID, BlockEntity*> Chunk::getNeighborBlock(const int x, const int y, const WorldLayer layer) const
{
	uchar dir = 0;
	if(x < 0) dir |= 0b1000;
	else if(x > CHUNK_BLOCKS - 1) dir |= 0b0100;
	if(y < 0) dir |= 0b0010;
	else if(y > CHUNK_BLOCKS - 1) dir |= 0b0001;
	if(dir == 0) return make_tuple(m_blocks[BLOCK_INDEX(x, y, layer)], m_blockEntities[BLOCK_INDEX(x, y, layer)]);

	Chunk *neighborChunk = m_neighborChunks[DIR_INDEX[dir]];
	if(neighborChunk)
	{
		return make_tuple(neighborChunk->m_blocks[BLOCK_INDEX(math::mod(x, CHUNK_BLOCKS), math::mod(y, CHUNK_BLOCKS), layer)], m_blockEntities[BLOCK_INDEX(math::mod(x, CHUNK_BLOCKS), math::mod(y, CHUNK_BLOCKS), layer)]);
	}
	return make_tuple(0, nullptr);
}
