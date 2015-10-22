#include "Chunk.h"
#include "Terrain.h"
#include "Constants.h"

#include "World/World.h"
#include "Blocks/BlockData.h"

#include "Generation/Generator.h"

#include "Entities/Static/StaticEntity.h"

// CONSTRUCTOR
Chunk::Chunk(ShaderPtr tileMapShader, ShaderPtr tileSortShader)
{
	// Setup flags and such
	m_dirty = m_modified = false; // Not modified
	m_shadowMap = Texture2DPtr(new Texture2D(Pixmap(CHUNK_BLOCKS, CHUNK_BLOCKS)));

	// Initialize blocks
	m_blocks = new BlockID[CHUNK_BLOCKS * CHUNK_BLOCKS * TERRAIN_LAYER_COUNT];
	for(int i = 0; i < CHUNK_BLOCKS * CHUNK_BLOCKS * TERRAIN_LAYER_COUNT; ++i)
	{
		m_blocks[i] = BLOCK_EMPTY;
	}

	// Initialize adjacency list
	for(int i = 0; i < 8; ++i)
	{
		m_adjacentChunks[i] = nullptr;
	}

	m_blockGridTexture = nullptr;
}

// BLOCK LOADING
void Chunk::load(int chunkX, int chunkY, BlockID *blocks)
{
	m_x = chunkX;
	m_y = chunkY;

	// Set all blocks
	for (int z = 0; z < TERRAIN_LAYER_COUNT; ++z)
	{
		for (int y = 0; y < CHUNK_BLOCKS; ++y)
		{
			for (int x = 0; x < CHUNK_BLOCKS; ++x)
			{
				m_blocks[BLOCK_INDEX(x, y, z)] = blocks[BLOCK_INDEX(x, y, z)];
			}
		}
	}

	// Load shadow map
	Pixmap pixmap(CHUNK_BLOCKS, CHUNK_BLOCKS);
	uchar pixel[4];
	for(int y = 0; y < CHUNK_BLOCKS; ++y)
	{
		for(int x = 0; x < CHUNK_BLOCKS; ++x)
		{
			float shadow = 1.0f;
			bool shadowCaster = m_blocks[BLOCK_INDEX(x, y, TERRAIN_LAYER_MIDDLE)] <= BLOCK_ENTITY;
			for(uint i = 0; i < TERRAIN_LAYER_COUNT; ++i)
			{
				shadow -= BlockData::get(m_blocks[BLOCK_INDEX(x, y, i)]).getOpacity();
			}
			pixel[0] = uchar(255 * max(shadow, 0.0f));
			pixel[1] = 255;
			pixel[2] = 255;
			pixel[3] = 255 * shadowCaster;
			pixmap.setPixel(x, CHUNK_BLOCKS - y - 1, pixel); // rgb = light value, a = shadow casting value
		}
	}
	m_shadowMap->updatePixmap(pixmap);
		
	// Mark as dirty
	m_dirty = true;

	// Mark as not modified
	m_modified = false;

	// Mark chunk as initialized
	LOG("Chunk [%i, %i] generated", m_x, m_y);
}

void Chunk::updateTileMap(ChunkLoader *chunkLoader)
{
	// Get adjacent chunks
	m_adjacentChunks[0] = &chunkLoader->getChunkAt(m_x - 1, m_y - 1);
	m_adjacentChunks[1] = &chunkLoader->getChunkAt(m_x, m_y - 1);
	m_adjacentChunks[2] = &chunkLoader->getChunkAt(m_x + 1, m_y - 1);
	m_adjacentChunks[3] = &chunkLoader->getChunkAt(m_x + 1, m_y);
	m_adjacentChunks[4] = &chunkLoader->getChunkAt(m_x + 1, m_y + 1);
	m_adjacentChunks[5] = &chunkLoader->getChunkAt(m_x, m_y + 1);
	m_adjacentChunks[6] = &chunkLoader->getChunkAt(m_x - 1, m_y + 1);
	m_adjacentChunks[7] = &chunkLoader->getChunkAt(m_x - 1, m_y);

	// Load tile map
	Pixmap pixmap(CHUNK_BLOCKS, CHUNK_BLOCKS);
	uchar pixel[4];
	pixel[3] = 0;
	for (int y = 0; y < CHUNK_BLOCKS; y++)
	{
		for (int x = 0; x < CHUNK_BLOCKS; x++)
		{
			for(int z = 0; z < TERRAIN_LAYER_COUNT; ++z)
			{
				pixel[z] = m_blocks[BLOCK_INDEX(x, y, z)];
			}
			pixmap.setPixel(x, CHUNK_BLOCKS - y - 1, pixel);
		}
	}

	// Create tile map texture
	m_blockGridTexture = Texture2DPtr(new Texture2D(pixmap));

	// Not dirty
	m_dirty = false;
}

// BLOCKS
BlockID Chunk::getBlockAt(const int x, const int y, TerrainLayer layer) const
{
	return m_blocks[BLOCK_INDEX(x, y, layer)];
}
	
bool Chunk::isBlockAt(const int x, const int y, TerrainLayer layer) const
{
	return m_blocks[BLOCK_INDEX(x, y, layer)] != BLOCK_EMPTY;
}
	
bool Chunk::isBlockOccupied(const int x, const int y, TerrainLayer layer) const
{
	return m_blocks[BLOCK_INDEX(x, y, layer)] >= BLOCK_ENTITY;
}
	
bool Chunk::setBlockAt(const int x, const int y, const BlockID block, TerrainLayer layer)
{
	// Make sure we can add a block here
	if(m_blocks[BLOCK_INDEX(x, y, layer)] != block)
	{
		// Set the block value
		m_blocks[BLOCK_INDEX(x, y, layer)] = block; // TODO: It might be more efficient to also change the m_blockGridTexture directly instead of marking the chunk as dirty
		m_dirty = m_modified = true; // Mark chunk as modified
		if(m_adjacentChunks[0] && x == 0 && y == 0)								m_adjacentChunks[0]->m_dirty = true;
		if(m_adjacentChunks[1] && y == 0)										m_adjacentChunks[1]->m_dirty = true;
		if(m_adjacentChunks[2] && x == CHUNK_BLOCKS-1 && y == 0)				m_adjacentChunks[2]->m_dirty = true;
		if(m_adjacentChunks[3] && x == CHUNK_BLOCKS-1)							m_adjacentChunks[3]->m_dirty = true;
		if(m_adjacentChunks[4] && x == CHUNK_BLOCKS-1 && y == CHUNK_BLOCKS-1)	m_adjacentChunks[4]->m_dirty = true;
		if(m_adjacentChunks[5] && y == CHUNK_BLOCKS-1)							m_adjacentChunks[5]->m_dirty = true;
		if(m_adjacentChunks[6] && x == 0 && y == CHUNK_BLOCKS-1)				m_adjacentChunks[6]->m_dirty = true;
		if(m_adjacentChunks[7] && x == 0)										m_adjacentChunks[7]->m_dirty = true;
		
		// Update shadow map
		float shadow = 1.0f;
		bool shadowCaster = m_blocks[BLOCK_INDEX(x, y, TERRAIN_LAYER_MIDDLE)] <= BLOCK_ENTITY;
		for(uint i = 0; i < TERRAIN_LAYER_COUNT; ++i)
		{
			shadow -= BlockData::get(m_blocks[BLOCK_INDEX(x, y, i)]).getOpacity();
		}
		const uchar pixel[4] = { (uchar) (255 * max(shadow, 0.0f)), 255, 255, (uchar) (255 * shadowCaster) };
		m_shadowMap->updatePixmap(x, CHUNK_BLOCKS - y - 1, Pixmap(1, 1, pixel));

		return true; // Return true as something was changed
	}
	return false; // Nothing changed
}

void Chunk::addStaticEntity(StaticEntity * entity)
{
	m_staticEntitites.insert(entity);
}

// DRAWING
void Chunk::drawBlocks(GraphicsContext &context, ChunkLoader *chunkLoader)
{
	// Should we generate a new tile map?
	if(m_dirty)
	{
		updateTileMap(chunkLoader);
	}

	// Draw blocks
	context.setTexture(m_blockGridTexture);
	context.drawRectangle(0, 0, CHUNK_BLOCKS, CHUNK_BLOCKS);
	context.setTexture(0);
}