#include "Chunk.h"
#include "ChunkManager.h"
#include "Constants.h"
#include "World/World.h"
#include "Generation/Generator.h"

Chunk::Chunk(ChunkManager *chunkManager) :
	m_chunkManager(chunkManager),
	m_generateBlockEntityBuffers(false)
{
	// Setup flags and such
	m_attached = m_modified = m_sorted = false; // Not modified

	// Initialize blocks
	m_blocks = new Block[CHUNK_BLOCKS * CHUNK_BLOCKS * WORLD_LAYER_COUNT];
	for(int i = 0; i < CHUNK_BLOCKS * CHUNK_BLOCKS * WORLD_LAYER_COUNT; ++i)
	{
		m_blocks[i] = BLOCK_EMPTY;
	}
	m_blockTexture = Resource<Texture2D>(new Texture2D(CHUNK_BLOCKS, CHUNK_BLOCKS));

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
	m_timeOffsetTexture = Resource<Texture2D>(new Texture2D(pixmap));
}

void Chunk::load(int chunkX, int chunkY, Block *blocks)
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
				BlockData *data = m_blocks[BLOCK_INDEX(x, y, z)].getBlockData();
				pixel[z] = (uchar) data->getID();
				shadow -= data->getOpacity();
			}
			pixel[3] = uchar(255 * max(shadow, 0.0f));
			pixmap.setPixel(x, y, pixel);
		}
	}

	// Create tile map texture
	m_blockTexture->updatePixmap(pixmap);

	// Mark as not modified
	m_attached = m_modified = false;

	// Generate block entity buffers
	m_generateBlockEntityBuffers = true;

	// Chunk generated
	LOG("Chunk [%i, %i] generated", m_x, m_y);
}

Block Chunk::getBlockAt(const int x, const int y, WorldLayer layer) const
{
	return m_blocks[BLOCK_INDEX(x, y, layer)];
}
	
bool Chunk::isBlockAt(const int x, const int y, WorldLayer layer) const
{
	return m_blocks[BLOCK_INDEX(x, y, layer)] != BLOCK_EMPTY;
}
	
bool Chunk::isBlockOccupied(const int x, const int y, WorldLayer layer) const
{
	return m_blocks[BLOCK_INDEX(x, y, layer)] >= BLOCK_ENTITY;
}
	
bool Chunk::setBlockAt(const int x, const int y, const Block block, WorldLayer layer)
{
	// Make sure we can add a block here
	if(m_blocks[BLOCK_INDEX(x, y, layer)] != block)
	{
		// Get adjacent chunks
		Chunk *neighborChunks[8];
		neighborChunks[0] = &m_chunkManager->getChunkAt(m_x - 1, m_y - 1);
		neighborChunks[1] = &m_chunkManager->getChunkAt(m_x, m_y - 1);
		neighborChunks[2] = &m_chunkManager->getChunkAt(m_x + 1, m_y - 1);
		neighborChunks[3] = &m_chunkManager->getChunkAt(m_x + 1, m_y);
		neighborChunks[4] = &m_chunkManager->getChunkAt(m_x + 1, m_y + 1);
		neighborChunks[5] = &m_chunkManager->getChunkAt(m_x, m_y + 1);
		neighborChunks[6] = &m_chunkManager->getChunkAt(m_x - 1, m_y + 1);
		neighborChunks[7] = &m_chunkManager->getChunkAt(m_x - 1, m_y);

		Block *neighborBlocks[8];
		if(neighborChunks[0] && x == 0 && y == 0)
		{
			neighborBlocks[0] = &neighborChunks[0]->m_blocks[BLOCK_INDEX(CHUNK_BLOCKS - 1, CHUNK_BLOCKS - 1, layer)];
			neighborChunks[0]->m_attached = true; // TODO: false?
		}
		else
		{
			neighborBlocks[0] = &m_blocks[BLOCK_INDEX(x - 1, y - 1, layer)];
		}

		if(neighborChunks[1] && y == 0)
		{
			neighborBlocks[1] = &neighborChunks[1]->m_blocks[BLOCK_INDEX(x, CHUNK_BLOCKS - 1, layer)];
			neighborChunks[1]->m_attached = true;
		}
		else
		{
			neighborBlocks[1] = &m_blocks[BLOCK_INDEX(x, y - 1, layer)];
		}

		if(neighborChunks[2] && x == CHUNK_BLOCKS - 1 && y == 0)
		{
			neighborBlocks[2] = &neighborChunks[2]->m_blocks[BLOCK_INDEX(x, CHUNK_BLOCKS - 1, layer)];
			neighborChunks[2]->m_attached = true;
		}
		else
		{
			neighborBlocks[2] = &m_blocks[BLOCK_INDEX(x + 1, y - 1, layer)];
		}

		if(neighborChunks[3] && x == CHUNK_BLOCKS - 1)
		{
			neighborBlocks[3] = &neighborChunks[3]->m_blocks[BLOCK_INDEX(0, y, layer)];
			neighborChunks[3]->m_attached = true;
		}
		else
		{
			neighborBlocks[3] = &m_blocks[BLOCK_INDEX(x + 1, y, layer)];
		}

		if(neighborChunks[4] && x == CHUNK_BLOCKS - 1 && y == CHUNK_BLOCKS - 1)
		{
			neighborBlocks[4] = &neighborChunks[4]->m_blocks[BLOCK_INDEX(0, 0, layer)];
			neighborChunks[4]->m_attached = true;
		}
		else
		{
			neighborBlocks[4] = &m_blocks[BLOCK_INDEX(x + 1, y + 1, layer)];
		}

		if(neighborChunks[5] && y == CHUNK_BLOCKS - 1)
		{
			neighborBlocks[5] = &neighborChunks[5]->m_blocks[BLOCK_INDEX(x, 0, layer)];
			neighborChunks[5]->m_attached = true;
		}
		else
		{
			neighborBlocks[5] = &m_blocks[BLOCK_INDEX(x, y + 1, layer)];
		}

		if(neighborChunks[6] && x == 0 && y == CHUNK_BLOCKS - 1)
		{
			neighborBlocks[6] = &neighborChunks[6]->m_blocks[BLOCK_INDEX(CHUNK_BLOCKS - 1, 0, layer)];
			neighborChunks[6]->m_attached = true;
		}
		else
		{
			neighborBlocks[6] = &m_blocks[BLOCK_INDEX(x - 1, y + 1, layer)];
		}

		if(neighborChunks[7] && x == 0)
		{
			neighborBlocks[7] = &neighborChunks[7]->m_blocks[BLOCK_INDEX(CHUNK_BLOCKS - 1, y, layer)];
			neighborChunks[7]->m_attached = true;
		}
		else
		{
			neighborBlocks[7] = &m_blocks[BLOCK_INDEX(x - 1, y, layer)];
		}
		
		// Notify block entities
		const int dirX[8] = {  1,  0, -1, -1, -1,  0,  1,  1 };
		const int dirY[8] = {  1,  1,  1,  0, -1, -1, -1,  0 };
		for(int i = 0; i < 8; i++)
		{
			if(*neighborBlocks[i] == BLOCK_ENTITY)
			{
				NeighborChangedEvent e(dirX[i], dirY[i], &m_blocks[BLOCK_INDEX(x, y, layer)], &block);
				neighborBlocks[i]->getBlockEntity()->onNeighbourChanged(&e);
			}
		}

		// Set the block value
		m_blocks[BLOCK_INDEX(x, y, layer)] = block;
		m_sorted = m_attached = false; m_modified = true; // Mark chunk as modified

		// Update block map
		uchar pixel[4];
		float shadow = 1.0f;
		for(int z = 0; z < WORLD_LAYER_COUNT; ++z)
		{
			BlockData *data = m_blocks[BLOCK_INDEX(x, y, z)].getBlockData();
			pixel[z] = (uchar) data->getID();
			shadow -= data->getOpacity();
		}
		pixel[3] = (uchar) (255 * max(shadow, 0.0f));
		m_blockTexture->updatePixmap(x, y, Pixmap(1, 1, pixel));

		return true; // Return true as something was changed
	}
	return false; // Nothing changed
}

bool Chunk::addBlockEntity(const int x, const int y, const Block block, WorldLayer layer)
{
	m_blockEntities.push_back(block.getBlockEntity());
	m_generateBlockEntityBuffers = true;
	return true;
}

bool Chunk::removeBlockEntity(BlockEntity *blockEntity)
{
	m_blockEntities.remove(blockEntity);
	delete blockEntity;
	m_generateBlockEntityBuffers = true;
	return true;
}

void Chunk::setBlockEntityFrameAt(const int x, const int y, const uint frame, const WorldLayer layer)
{
	uchar data[4];
	data[0] = frame;
	data[1] = data[2] = data[3] = 0;
	m_timeOffsetTexture->updatePixmap(x, y, Pixmap(1, 1, data));
	LOG("%i", frame);
}

void Chunk::attach(GraphicsContext *context, const int x, const int y)
{
	// Draw blocks
	context->setTexture(m_blockTexture);
	context->drawRectangle(x * CHUNK_BLOCKS, y * CHUNK_BLOCKS, CHUNK_BLOCKS, CHUNK_BLOCKS);
	context->setTexture(0);

	// TODO: Render block entities to the global surface?

	m_attached = true;
}

void Chunk::detach()
{
	m_sorted = m_attached = false;
}

void Chunk::drawBlockEntities(GraphicsContext *context)
{
	if(m_generateBlockEntityBuffers)
	{
		Vertex *vertices = new Vertex[m_blockEntities.size() * 4];
		uint *indices = new uint[m_blockEntities.size() * 6];
		int i = 0;
		for(BlockEntity *blockEntity : m_blockEntities)
		{
			blockEntity->getVertices(vertices + i * 4, indices + i * 6, i);
			i++;
		}

		m_blockEntityVBO.setData(vertices, m_blockEntities.size() * 4);
		m_blockEntityIBO.setData(indices, m_blockEntities.size() * 6);
		delete[] vertices;

		m_generateBlockEntityBuffers = false;
	}

	// TODO: I should store BlockEntities in a global buffer of some sort.
	context->drawIndexedPrimitives(GraphicsContext::PRIMITIVE_TRIANGLES, &m_blockEntityVBO, &m_blockEntityIBO);
}
