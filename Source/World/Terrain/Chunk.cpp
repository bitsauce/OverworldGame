#include "Chunk.h"
#include "Terrain.h"
#include "Constants.h"

#include "World/World.h"
#include "Blocks/BlockData.h"

#include "Generation/Generator.h"

#include "BlockEntities/BlockEntity.h"

// CONSTRUCTOR
Chunk::Chunk(ChunkLoader *chunkLoader) :
	m_chunkLoader(chunkLoader)
{
	// Setup flags and such
	m_attached = m_modified = false; // Not modified

	// Initialize blocks
	m_blocks = new BlockID[CHUNK_BLOCKS * CHUNK_BLOCKS * WORLD_LAYER_COUNT];
	for(int i = 0; i < CHUNK_BLOCKS * CHUNK_BLOCKS * WORLD_LAYER_COUNT; ++i)
	{
		m_blocks[i] = BLOCK_EMPTY;
	}
	m_blockTexture = Texture2DPtr(new Texture2D(CHUNK_BLOCKS, CHUNK_BLOCKS));
}

// BLOCK LOADING
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
				pixel[z] = (uchar) m_blocks[BLOCK_INDEX(x, y, z)];
				shadow -= BlockData::get(m_blocks[BLOCK_INDEX(x, y, z)]).getOpacity();
			}
			pixel[3] = uchar(255 * max(shadow, 0.0f));
			pixmap.setPixel(x, CHUNK_BLOCKS - y - 1, pixel);
		}
	}

	// Create tile map texture
	m_blockTexture->updatePixmap(pixmap);

	// Mark as not modified
	m_attached = m_modified = false;

	// Chunk generated
	LOG("Chunk [%i, %i] generated", m_x, m_y);
}

// BLOCKS
BlockID Chunk::getBlockAt(const int x, const int y, WorldLayer layer) const
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
	
bool Chunk::setBlockAt(const int x, const int y, const BlockID block, WorldLayer layer)
{
	// Make sure we can add a block here
	if(m_blocks[BLOCK_INDEX(x, y, layer)] != block)
	{
		// Get adjacent chunks
		Chunk *neighborChunks[8];
		neighborChunks[0] = &m_chunkLoader->getChunkAt(m_x - 1, m_y - 1);
		neighborChunks[1] = &m_chunkLoader->getChunkAt(m_x, m_y - 1);
		neighborChunks[2] = &m_chunkLoader->getChunkAt(m_x + 1, m_y - 1);
		neighborChunks[3] = &m_chunkLoader->getChunkAt(m_x + 1, m_y);
		neighborChunks[4] = &m_chunkLoader->getChunkAt(m_x + 1, m_y + 1);
		neighborChunks[5] = &m_chunkLoader->getChunkAt(m_x, m_y + 1);
		neighborChunks[6] = &m_chunkLoader->getChunkAt(m_x - 1, m_y + 1);
		neighborChunks[7] = &m_chunkLoader->getChunkAt(m_x - 1, m_y);

		// Set the block value
		m_blocks[BLOCK_INDEX(x, y, layer)] = block; // TODO: It might be more efficient to also change the m_blockTexture directly instead of marking the chunk as dirty
		m_attached = false; m_modified = true; // Mark chunk as modified
		if(neighborChunks[0] && x == 0 && y == 0)							neighborChunks[0]->m_attached = true;
		if(neighborChunks[1] && y == 0)										neighborChunks[1]->m_attached = true;
		if(neighborChunks[2] && x == CHUNK_BLOCKS-1 && y == 0)				neighborChunks[2]->m_attached = true;
		if(neighborChunks[3] && x == CHUNK_BLOCKS-1)						neighborChunks[3]->m_attached = true;
		if(neighborChunks[4] && x == CHUNK_BLOCKS-1 && y == CHUNK_BLOCKS-1)	neighborChunks[4]->m_attached = true;
		if(neighborChunks[5] && y == CHUNK_BLOCKS-1)						neighborChunks[5]->m_attached = true;
		if(neighborChunks[6] && x == 0 && y == CHUNK_BLOCKS-1)				neighborChunks[6]->m_attached = true;
		if(neighborChunks[7] && x == 0)										neighborChunks[7]->m_attached = true;
		
		// Update block map
		uchar pixel[4];
		float shadow = 1.0f;
		for(int z = 0; z < WORLD_LAYER_COUNT; ++z)
		{
			pixel[z] = (uchar) m_blocks[BLOCK_INDEX(x, y, z)];
			shadow -= BlockData::get(m_blocks[BLOCK_INDEX(x, y, z)]).getOpacity();
		}
		pixel[3] = (uchar) (255 * max(shadow, 0.0f));
		m_blockTexture->updatePixmap(x, CHUNK_BLOCKS - y - 1, Pixmap(1, 1, pixel));

		return true; // Return true as something was changed
	}
	return false; // Nothing changed
}

void Chunk::update(const float dt)
{
	for(BlockEntity *blockEntity : m_blockEntities)
	{
		blockEntity->update(dt);
	}
}

void Chunk::onDraw(DrawEvent *e)
{
	for(BlockEntity *blockEntity : m_blockEntities)
	{
		blockEntity->draw(spriteBatch, alpha);
	}
}

void Chunk::addStaticEntity(BlockEntity * entity)
{
	m_blockEntities.insert(entity);
}

// DRAWING
void Chunk::attach(GraphicsContext &context, const int x, const int y)
{
	// Draw blocks
	context.setTexture(m_blockTexture);
	context.drawRectangle(x * CHUNK_BLOCKS, y * CHUNK_BLOCKS, CHUNK_BLOCKS, CHUNK_BLOCKS);
	context.setTexture(0);

	m_attached = true;
}

void Chunk::detach()
{
	m_attached = false;
}