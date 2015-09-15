#include "Chunk.h"
#include "Terrain.h"
#include "Constants.h"

#include "World/World.h"
#include "Blocks/BlockData.h"

#include "Generation/Generator.h"

bool exported = false;

// CONSTRUCTOR
Chunk::Chunk()
{
	// Setup flags and such
	m_dirty[TERRAIN_LAYER_BACK] = m_dirty[TERRAIN_LAYER_MIDDLE] = m_dirty[TERRAIN_LAYER_FRONT] = m_modified = false; // not modified
	m_shadowMap = Texture2DPtr(new Texture2D(Pixmap(CHUNK_BLOCKS, CHUNK_BLOCKS)));
	m_tileMapShader = ResourceManager::get<Shader>(":/Shaders/TileMap");
	if (!exported) {
		exported = true;
	m_tileMapShader->exportAssembly(":/Shaders/TileMap.bin");
}

	// Initialize blocks
	m_blocks = new BlockID[CHUNK_BLOCKS*CHUNK_BLOCKS*TERRAIN_LAYER_COUNT];
	for(int z = 0; z < TERRAIN_LAYER_COUNT; ++z)
	{
		for(int y = 0; y < CHUNK_BLOCKS; ++y)
		{
			for(int x = 0; x < CHUNK_BLOCKS; ++x)
			{
				m_blocks[BLOCK_INDEX(x, y, z)] = BLOCK_EMPTY;
			}
		}
	}

	// Initialize adjacency list
	for(int i = 0; i < 8; ++i)
	{
		m_adjacentChunks[i] = nullptr;
	}
}

// BLOCK LOADING
void Chunk::load(int chunkX, int chunkY, BlockID *blocks)
{
	m_dirty[TERRAIN_LAYER_BACK] = m_dirty[TERRAIN_LAYER_MIDDLE] = m_dirty[TERRAIN_LAYER_FRONT] = m_modified = false; // not modified
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
			pixel[0] = 255 * max(shadow, 0.0f);
			pixel[1] = 255;
			pixel[2] = 255;
			pixel[3] = 255 * shadowCaster;
			pixmap.setPixel(x, CHUNK_BLOCKS - y - 1, pixel); // rgb = light value, a = shadow casting value
		}
	}
	m_shadowMap->updatePixmap(pixmap);
		
	// Mark as dirty
	m_dirty[TERRAIN_LAYER_BACK] = m_dirty[TERRAIN_LAYER_MIDDLE] = m_dirty[TERRAIN_LAYER_FRONT] = true;

	// Mark chunk as initialized
	LOG("Chunk [%i, %i] generated", m_x, m_y);
}

void Chunk::updateTileMap(ChunkLoader *chunkLoader, TerrainLayer z)
{
	// Get adjacent chunks
	m_adjacentChunks[0] = &chunkLoader->getChunkAt(m_x-1, m_y-1);
	m_adjacentChunks[1] = &chunkLoader->getChunkAt(m_x,   m_y-1);
	m_adjacentChunks[2] = &chunkLoader->getChunkAt(m_x+1, m_y-1);
	m_adjacentChunks[3] = &chunkLoader->getChunkAt(m_x+1, m_y  );
	m_adjacentChunks[4] = &chunkLoader->getChunkAt(m_x+1, m_y+1);
	m_adjacentChunks[5] = &chunkLoader->getChunkAt(m_x,   m_y+1);
	m_adjacentChunks[6] = &chunkLoader->getChunkAt(m_x-1, m_y+1);
	m_adjacentChunks[7] = &chunkLoader->getChunkAt(m_x-1, m_y  );

	// Load tile map
	Pixmap pixmap(CHUNK_BLOCKS + 2, CHUNK_BLOCKS + 2, PixelFormat(PixelFormat::RGBA, PixelFormat::UNSIGNED_INT));
	uint pixel[4];
	pixel[1] = pixel[2] = pixel[3] = 0;
	for (int y = 0; y < CHUNK_BLOCKS; y++)
	{
		for (int x = 0; x < CHUNK_BLOCKS; x++)
		{
			pixel[0] = m_blocks[BLOCK_INDEX(x, y, TERRAIN_LAYER_MIDDLE)];
			pixmap.setPixel(x + 1, CHUNK_BLOCKS - y, pixel);
		}
	}

	// Top-left
	pixel[0] = m_adjacentChunks[0]->m_blocks[BLOCK_INDEX(CHUNK_BLOCKS - 1, CHUNK_BLOCKS - 1, TERRAIN_LAYER_MIDDLE)];
	pixmap.setPixel(0, CHUNK_BLOCKS + 1, pixel);

	// Top-right
	pixel[0] = m_adjacentChunks[2]->m_blocks[BLOCK_INDEX(0, CHUNK_BLOCKS - 1, TERRAIN_LAYER_MIDDLE)];
	pixmap.setPixel(CHUNK_BLOCKS + 1, CHUNK_BLOCKS + 1, pixel);

	// Bottom-right
	pixel[0] = m_adjacentChunks[4]->m_blocks[BLOCK_INDEX(0, 0, TERRAIN_LAYER_MIDDLE)];
	pixmap.setPixel(CHUNK_BLOCKS + 1, 0, pixel);

	// Bottom-left
	pixel[0] = m_adjacentChunks[6]->m_blocks[BLOCK_INDEX(CHUNK_BLOCKS - 1, 0, TERRAIN_LAYER_MIDDLE)];
	pixmap.setPixel(0, 0, pixel);

	// Top
	for (int i = 0; i < CHUNK_BLOCKS; ++i)
	{
		pixel[0] = m_adjacentChunks[1]->m_blocks[BLOCK_INDEX(i, CHUNK_BLOCKS - 1, TERRAIN_LAYER_MIDDLE)];
		pixmap.setPixel(i + 1, CHUNK_BLOCKS + 1, pixel);
	}

	// Bottom
	for (int i = 0; i < CHUNK_BLOCKS; ++i)
	{
		pixel[0] = m_adjacentChunks[5]->m_blocks[BLOCK_INDEX(i, 0, TERRAIN_LAYER_MIDDLE)];
		pixmap.setPixel(i + 1, 0, pixel);
	}

	// Left
	for (int i = 0; i < CHUNK_BLOCKS; ++i)
	{
		pixel[0] = m_adjacentChunks[7]->m_blocks[BLOCK_INDEX(CHUNK_BLOCKS - 1, i, TERRAIN_LAYER_MIDDLE)];
		pixmap.setPixel(0, CHUNK_BLOCKS - i, pixel);
	}

	// Right
	for (int i = 0; i < CHUNK_BLOCKS; ++i)
	{
		pixel[0] = m_adjacentChunks[3]->m_blocks[BLOCK_INDEX(0, i, TERRAIN_LAYER_MIDDLE)];
		pixmap.setPixel(CHUNK_BLOCKS + 1, CHUNK_BLOCKS - i, pixel);
	}

	m_tileMapTexture = Texture2DPtr(new Texture2D(pixmap));
	m_tileMapShader->setSampler2D("u_BlockAtlas", BlockData::getBlockAtlas()->getTexture());

	// Not dirty
	m_dirty[z] = false;
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
		m_blocks[BLOCK_INDEX(x, y, layer)] = block;
		m_dirty[layer] = m_modified = true; // mark chunk as modified
		if(m_adjacentChunks[0] && x == 0 && y == 0)								m_adjacentChunks[0]->m_dirty[layer] = true;
		if(m_adjacentChunks[1] && y == 0)										m_adjacentChunks[1]->m_dirty[layer] = true;
		if(m_adjacentChunks[2] && x == CHUNK_BLOCKS-1 && y == 0)				m_adjacentChunks[2]->m_dirty[layer] = true;
		if(m_adjacentChunks[3] && x == CHUNK_BLOCKS-1)							m_adjacentChunks[3]->m_dirty[layer] = true;
		if(m_adjacentChunks[4] && x == CHUNK_BLOCKS-1 && y == CHUNK_BLOCKS-1)	m_adjacentChunks[4]->m_dirty[layer] = true;
		if(m_adjacentChunks[5] && y == CHUNK_BLOCKS-1)							m_adjacentChunks[5]->m_dirty[layer] = true;
		if(m_adjacentChunks[6] && x == 0 && y == CHUNK_BLOCKS-1)				m_adjacentChunks[6]->m_dirty[layer] = true;
		if(m_adjacentChunks[7] && x == 0)										m_adjacentChunks[7]->m_dirty[layer] = true;
		
		// Update shadow map
		float shadow = 1.0f;
		bool shadowCaster = m_blocks[BLOCK_INDEX(x, y, TERRAIN_LAYER_MIDDLE)] <= BLOCK_ENTITY;
		for(uint i = 0; i < TERRAIN_LAYER_COUNT; ++i)
		{
			shadow -= BlockData::get(m_blocks[BLOCK_INDEX(x, y, i)]).getOpacity();
		}
		const uchar pixel[4] = { 255 * max(shadow, 0.0f), 255, 255, 255 * shadowCaster };
		m_shadowMap->updatePixmap(x, CHUNK_BLOCKS - y - 1, Pixmap(1, 1, pixel));

		return true; // Return true as something was changed
	}
	return false; // Nothing changed
}

// DRAWING
void Chunk::draw(GraphicsContext &gfxContext, const TerrainLayer layer)
{
	m_tileMapShader->setSampler2D("u_TileMap", m_tileMapTexture);

	gfxContext.setShader(m_tileMapShader);

	Vertex vertices[4];

	int x = m_x * CHUNK_PXF,
		y = m_y * CHUNK_PXF,
		width = CHUNK_PXF,
		height = CHUNK_PXF;

	vertices[0].set4f(xd::VERTEX_POSITION, x, y);
	vertices[1].set4f(xd::VERTEX_POSITION, x, y + height);
	vertices[2].set4f(xd::VERTEX_POSITION, x + width, y);
	vertices[3].set4f(xd::VERTEX_POSITION, x + width, y + height);

	Color color(255);

	vertices[0].set4ub(xd::VERTEX_COLOR, color.r, color.g, color.b, color.a);
	vertices[1].set4ub(xd::VERTEX_COLOR, color.r, color.g, color.b, color.a);
	vertices[2].set4ub(xd::VERTEX_COLOR, color.r, color.g, color.b, color.a);
	vertices[3].set4ub(xd::VERTEX_COLOR, color.r, color.g, color.b, color.a);

	float u0 = 1.0f / (CHUNK_BLOCKSF + 2.0),
		v0 = 1.0f - (1.0f / (CHUNK_BLOCKSF + 2.0)),
		u1 = 1.0f - (1.0f / (CHUNK_BLOCKSF + 2.0)),
		v1 = 1.0f / (CHUNK_BLOCKSF + 2.0);

	vertices[0].set4f(xd::VERTEX_TEX_COORD, u0, v0);
	vertices[1].set4f(xd::VERTEX_TEX_COORD, u0, v1);
	vertices[2].set4f(xd::VERTEX_TEX_COORD, u1, v0);
	vertices[3].set4f(xd::VERTEX_TEX_COORD, u1, v1);

	gfxContext.drawPrimitives(GraphicsContext::PRIMITIVE_TRIANGLE_STRIP, vertices, 4);

	gfxContext.setShader(0);
}