#include "Chunk.h"
#include "Terrain.h"
#include "Constants.h"

#include "World/World.h"
#include "Blocks/BlockData.h"

#include "Generation/Generator.h"

// CONSTRUCTOR
Chunk::Chunk(ShaderPtr tileMapShader, ShaderPtr tileSortShader) :
	m_tileMapShader(tileMapShader),
	m_tileSortShader(tileSortShader)
{
	// Setup flags and such
	m_dirty[TERRAIN_LAYER_BACK] = m_dirty[TERRAIN_LAYER_MIDDLE] = m_dirty[TERRAIN_LAYER_FRONT] =
		m_sorted[TERRAIN_LAYER_BACK] = m_sorted[TERRAIN_LAYER_MIDDLE] = m_sorted[TERRAIN_LAYER_FRONT] =
		m_modified = false; // not modified
	m_shadowMap = Texture2DPtr(new Texture2D(Pixmap(CHUNK_BLOCKS, CHUNK_BLOCKS)));
	for(uint i = 0; i < TERRAIN_LAYER_COUNT; ++i)
	{
		m_sortRenderTarget[i] = new RenderTarget2D(CHUNK_BLOCKS + 2, CHUNK_BLOCKS + 2, 2, PixelFormat(PixelFormat::RGBA, PixelFormat::UNSIGNED_INT));
	}

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
}

/*if(i == QUAD_0)  return vec2(1.0, 1.0) + vec2(0.0, 24.0) + vec2(34.0 * float(blockID), 0.0);
if(i == QUAD_1)  return vec2(1.0, 1.0) + vec2(8.0, 24.0) + vec2(34.0 * float(blockID), 0.0);
if(i == QUAD_2)  return vec2(1.0, 1.0) + vec2(16.0, 24.0) + vec2(34.0 * float(blockID), 0.0);
if(i == QUAD_3)  return vec2(1.0, 1.0) + vec2(24.0, 24.0) + vec2(34.0 * float(blockID), 0.0);
if(i == QUAD_4)  return vec2(1.0, 1.0) + vec2(0.0, 16.0) + vec2(34.0 * float(blockID), 0.0);
if(i == QUAD_5)  return vec2(1.0, 1.0) + vec2(8.0, 16.0) + vec2(34.0 * float(blockID), 0.0);
if(i == QUAD_6)  return vec2(1.0, 1.0) + vec2(16.0, 16.0) + vec2(34.0 * float(blockID), 0.0);
if(i == QUAD_7)  return vec2(1.0, 1.0) + vec2(24.0, 16.0) + vec2(34.0 * float(blockID), 0.0);
if(i == QUAD_8)  return vec2(1.0, 1.0) + vec2(0.0, 8.0) + vec2(34.0 * float(blockID), 0.0);
if(i == QUAD_9)  return vec2(1.0, 1.0) + vec2(8.0, 8.0) + vec2(34.0 * float(blockID), 0.0);
if(i == QUAD_10) return vec2(1.0, 1.0) + vec2(16.0, 8.0) + vec2(34.0 * float(blockID), 0.0);
if(i == QUAD_11) return vec2(1.0, 1.0) + vec2(24.0, 8.0) + vec2(34.0 * float(blockID), 0.0);
if(i == QUAD_12) return vec2(1.0, 1.0) + vec2(0.0, 0.0) + vec2(34.0 * float(blockID), 0.0);
if(i == QUAD_13) return vec2(1.0, 1.0) + vec2(8.0, 0.0) + vec2(34.0 * float(blockID), 0.0);
if(i == QUAD_14) return vec2(1.0, 1.0) + vec2(16.0, 0.0) + vec2(34.0 * float(blockID), 0.0);
if(i == QUAD_15) return vec2(1.0, 1.0) + vec2(24.0, 0.0) + vec2(34.0 * float(blockID), 0.0);
if(i == QUAD_16) return vec2(1.0, 1.0) + vec2(0.0, 40.0) + vec2(34.0 * float(blockID), 0.0);
if(i == QUAD_17) return vec2(1.0, 1.0) + vec2(8.0, 40.0) + vec2(34.0 * float(blockID), 0.0);
if(i == QUAD_18) return vec2(1.0, 1.0) + vec2(0.0, 32.0) + vec2(34.0 * float(blockID), 0.0);
if(i == QUAD_19) return vec2(1.0, 1.0) + vec2(8.0, 32.0) + vec2(34.0 * float(blockID), 0.0);
return vec2(0.0);*/

const float QUAD_UVS[40] =
{
	1, 25,
	9, 25,
	17, 25,
	25, 25,

	1, 17,
	9, 17,
	17, 17,
	25, 17,

	1, 9,
	9, 9,
	17, 9,
	25, 9,

	1, 1,
	9, 1,
	17, 1,
	25, 1,

	1, 41,
	9, 41,
	1, 33,
	9, 33
};

// BLOCK LOADING
void Chunk::load(int chunkX, int chunkY, BlockID *blocks)
{
	m_dirty[TERRAIN_LAYER_BACK] = m_dirty[TERRAIN_LAYER_MIDDLE] = m_dirty[TERRAIN_LAYER_FRONT] =
		m_sorted[TERRAIN_LAYER_BACK] = m_sorted[TERRAIN_LAYER_MIDDLE] = m_sorted[TERRAIN_LAYER_FRONT] =
		m_modified = false; // not modified
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

	// Set block atlas
	m_tileMapShader->setSampler2D("u_BlockAtlas", BlockData::getBlockAtlas()->getTexture());
	m_tileMapShader->setUniform2fv("u_QuadUVs", QUAD_UVS);
		
	// Mark as dirty
	m_dirty[TERRAIN_LAYER_BACK] = m_dirty[TERRAIN_LAYER_MIDDLE] = m_dirty[TERRAIN_LAYER_FRONT] = true;

	// Mark chunk as initialized
	LOG("Chunk [%i, %i] generated", m_x, m_y);
}

void Chunk::updateTileMap(ChunkLoader *chunkLoader, TerrainLayer z)
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
	Pixmap pixmap(CHUNK_BLOCKS + 2, CHUNK_BLOCKS + 2, PixelFormat(PixelFormat::RGBA, PixelFormat::UNSIGNED_INT));
	uint pixel[4];
	pixel[1] = pixel[2] = pixel[3] = 0;
	for (int y = 0; y < CHUNK_BLOCKS; y++)
	{
		for (int x = 0; x < CHUNK_BLOCKS; x++)
		{
			pixel[0] = m_blocks[BLOCK_INDEX(x, y, z)];
			pixmap.setPixel(x + 1, CHUNK_BLOCKS - y, pixel);
		}
	}

	// Top-left
	pixel[0] = m_adjacentChunks[0]->m_blocks[BLOCK_INDEX(CHUNK_BLOCKS - 1, CHUNK_BLOCKS - 1, z)];
	pixmap.setPixel(0, CHUNK_BLOCKS + 1, pixel);

	// Top-right
	pixel[0] = m_adjacentChunks[2]->m_blocks[BLOCK_INDEX(0, CHUNK_BLOCKS - 1, z)];
	pixmap.setPixel(CHUNK_BLOCKS + 1, CHUNK_BLOCKS + 1, pixel);

	// Bottom-right
	pixel[0] = m_adjacentChunks[4]->m_blocks[BLOCK_INDEX(0, 0, z)];
	pixmap.setPixel(CHUNK_BLOCKS + 1, 0, pixel);

	// Bottom-left
	pixel[0] = m_adjacentChunks[6]->m_blocks[BLOCK_INDEX(CHUNK_BLOCKS - 1, 0, z)];
	pixmap.setPixel(0, 0, pixel);

	// Top
	for (int i = 0; i < CHUNK_BLOCKS; ++i)
	{
		pixel[0] = m_adjacentChunks[1]->m_blocks[BLOCK_INDEX(i, CHUNK_BLOCKS - 1, z)];
		pixmap.setPixel(i + 1, CHUNK_BLOCKS + 1, pixel);
	}

	// Bottom
	for (int i = 0; i < CHUNK_BLOCKS; ++i)
	{
		pixel[0] = m_adjacentChunks[5]->m_blocks[BLOCK_INDEX(i, 0, z)];
		pixmap.setPixel(i + 1, 0, pixel);
	}

	// Left
	for (int i = 0; i < CHUNK_BLOCKS; ++i)
	{
		pixel[0] = m_adjacentChunks[7]->m_blocks[BLOCK_INDEX(CHUNK_BLOCKS - 1, i, z)];
		pixmap.setPixel(0, CHUNK_BLOCKS - i, pixel);
	}

	// Right
	for (int i = 0; i < CHUNK_BLOCKS; ++i)
	{
		pixel[0] = m_adjacentChunks[3]->m_blocks[BLOCK_INDEX(0, i, z)];
		pixmap.setPixel(CHUNK_BLOCKS + 1, CHUNK_BLOCKS - i, pixel);
	}

	// Create tile map texture
	m_tileMapTexture[z] = Texture2DPtr(new Texture2D(pixmap));

	// Not dirty
	m_sorted[z] = m_dirty[z] = false;
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
		const uchar pixel[4] = { (uchar) (255 * max(shadow, 0.0f)), 255, 255, (uchar) (255 * shadowCaster) };
		m_shadowMap->updatePixmap(x, CHUNK_BLOCKS - y - 1, Pixmap(1, 1, pixel));

		return true; // Return true as something was changed
	}
	return false; // Nothing changed
}

#include "Entities/Static/StaticEntity.h"

void Chunk::addStaticEntity(StaticEntity * entity)
{
	m_staticEntitites.insert(entity);
}

// DRAWING
void Chunk::draw(GraphicsContext &context, const TerrainLayer layer)
{
	Vertex vertices[4];

	Color color(255);
	vertices[0].set4ub(xd::VERTEX_COLOR, color.r, color.g, color.b, color.a);
	vertices[1].set4ub(xd::VERTEX_COLOR, color.r, color.g, color.b, color.a);
	vertices[2].set4ub(xd::VERTEX_COLOR, color.r, color.g, color.b, color.a);
	vertices[3].set4ub(xd::VERTEX_COLOR, color.r, color.g, color.b, color.a);

	if(!m_sorted[layer])
	{
		vertices[0].set4f(xd::VERTEX_TEX_COORD, 0.0f, 1.0f);
		vertices[1].set4f(xd::VERTEX_TEX_COORD, 0.0f, 0.0f);
		vertices[2].set4f(xd::VERTEX_TEX_COORD, 1.0f, 1.0f);
		vertices[3].set4f(xd::VERTEX_TEX_COORD, 1.0f, 0.0f);

		vertices[0].set4f(xd::VERTEX_POSITION, 0,                0               );
		vertices[1].set4f(xd::VERTEX_POSITION, 0,                CHUNK_BLOCKS + 2);
		vertices[2].set4f(xd::VERTEX_POSITION, CHUNK_BLOCKS + 2, 0               );
		vertices[3].set4f(xd::VERTEX_POSITION, CHUNK_BLOCKS + 2, CHUNK_BLOCKS + 2);

		Matrix4 tmpMap = context.getModelViewMatrix();
		context.setModelViewMatrix(Matrix4());

		context.setRenderTarget(m_sortRenderTarget[layer]);
			context.setShader(m_tileSortShader);
				m_tileSortShader->setSampler2D("u_TileMap", m_tileMapTexture[layer]);
				context.drawPrimitives(GraphicsContext::PRIMITIVE_TRIANGLE_STRIP, vertices, 4);
			context.setShader(0);
		context.setRenderTarget(0);

		context.setModelViewMatrix(tmpMap);

		m_sorted[layer] = true;
	}

	float u0 = 1.0f / (CHUNK_BLOCKSF + 2.0f),
		v0 = 1.0f - (1.0f / (CHUNK_BLOCKSF + 2.0f)),
		u1 = 1.0f - (1.0f / (CHUNK_BLOCKSF + 2.0f)),
		v1 = 1.0f / (CHUNK_BLOCKSF + 2.0f);
	vertices[0].set4f(xd::VERTEX_TEX_COORD, u0, v0);
	vertices[1].set4f(xd::VERTEX_TEX_COORD, u0, v1);
	vertices[2].set4f(xd::VERTEX_TEX_COORD, u1, v0);
	vertices[3].set4f(xd::VERTEX_TEX_COORD, u1, v1);

	float x = m_x * CHUNK_PXF,
		y = m_y * CHUNK_PXF,
		width = CHUNK_PXF,
		height = CHUNK_PXF;
	vertices[0].set4f(xd::VERTEX_POSITION, x, y);
	vertices[1].set4f(xd::VERTEX_POSITION, x, y + height);
	vertices[2].set4f(xd::VERTEX_POSITION, x + width, y);
	vertices[3].set4f(xd::VERTEX_POSITION, x + width, y + height);

	context.setShader(m_tileMapShader);
		m_tileMapShader->setSampler2D("u_SortedBlockTexture", m_sortRenderTarget[layer]->getTexture(0));
		m_tileMapShader->setSampler2D("u_SortedQuadTexture", m_sortRenderTarget[layer]->getTexture(1));
		context.drawPrimitives(GraphicsContext::PRIMITIVE_TRIANGLE_STRIP, vertices, 4);
	context.setShader(0);

}