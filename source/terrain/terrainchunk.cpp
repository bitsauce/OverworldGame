#include "terrainchunk.h"
#include "terrain.h"
#include "constants.h"

#include "game/world.h"
#include "blocks/blockdata.h"

#include "generator/terraingen.h"

#define BLOCK_INDEX(x, y, z) (x) + (CHUNK_BLOCKS) * ((y) + (CHUNK_BLOCKS) * (z))

#define BLOCK_X0 0.00f
#define BLOCK_Y0 0.00f
#define BLOCK_X1 0.25f
#define BLOCK_Y1 0.25f
#define BLOCK_X2 0.50f
#define BLOCK_Y2 0.50f
#define BLOCK_X3 0.75f
#define BLOCK_Y3 0.75f
#define BLOCK_X4 1.00f
#define BLOCK_Y4 1.00f

#define BLOCK_U0 (0.0f / 6.0f)
#define BLOCK_V0 (0.0f / 10.0f)
#define BLOCK_U1 (1.0f / 6.0f)
#define BLOCK_V1 (1.0f / 10.0f)
#define BLOCK_U2 (2.0f / 6.0f)
#define BLOCK_V2 (2.0f / 10.0f)
#define BLOCK_U3 (3.0f / 6.0f)
#define BLOCK_V3 (3.0f / 10.0f)
#define BLOCK_U4 (4.0f / 6.0f)
#define BLOCK_V4 (4.0f / 10.0f)
#define BLOCK_U5 (5.0f / 6.0f)
#define BLOCK_V5 (5.0f / 10.0f)
#define BLOCK_U6 (6.0f / 6.0f)
#define BLOCK_V6 (6.0f / 10.0f)
#define BLOCK_V7 (8.0f / 10.0f)
#define BLOCK_V8 (10.0f / 10.0f)

Vertex *TerrainChunk::s_vertices = nullptr;
uint *TerrainChunk::s_indices = new uint[6*12*16*16*3];

// BLOCK QUADS
TerrainChunk::BlockQuad::BlockQuad() :
	block(BLOCK_EMPTY)
{
}

TerrainChunk::BlockQuad::BlockQuad(BlockID block, const float x0, const float y0, const float x1, const float y1, const float u0, const float v0, const float u1, const float v1) :
	block(block),
	x0(x0),
	y0(y0),
	x1(x1),
	y1(y1),
	u0(u0),
	v0(v0),
	u1(u1),
	v1(v1)
{
}

// CONSTRUCTOR
TerrainChunk::TerrainChunk()
{
	// A dummy
	m_dirty[TERRAIN_LAYER_BACK] = m_dirty[TERRAIN_LAYER_MIDDLE] = m_dirty[TERRAIN_LAYER_FRONT] = m_modified = false; // not modified
	m_shadowMap = Texture2DPtr(new Texture2D(Pixmap(CHUNK_BLOCKS, CHUNK_BLOCKS)));

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

// CHUNK LOADING
void TerrainChunk::load(int chunkX, int chunkY)
{
	m_x = chunkX;
	m_y = chunkY;

	// Set all blocks
	for(int z = 0; z < TERRAIN_LAYER_COUNT; ++z)
	{
		for(int y = 0; y < CHUNK_BLOCKS; ++y)
		{
			for(int x = 0; x < CHUNK_BLOCKS; ++x)
			{
				// Get block from TerrainGen
				m_blocks[BLOCK_INDEX(x, y, z)] = TerrainGen::getBlockAt(m_x * CHUNK_BLOCKS + x, m_y * CHUNK_BLOCKS + y, (TerrainLayer)z);
			}
		}
	}

	// Load shadow map
	Pixmap pixmap(CHUNK_BLOCKS, CHUNK_BLOCKS);
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
			pixmap.setColor(x, CHUNK_BLOCKS - y - 1, Color(255 * max(shadow, 0.0f), 255, 255, 255 * shadowCaster)); // rgb = light value, a = shadow casting value
		}
	}
	m_shadowMap->updatePixmap(pixmap);
		
	// Mark as dirty
	m_dirty[TERRAIN_LAYER_BACK] = m_dirty[TERRAIN_LAYER_MIDDLE] = m_dirty[TERRAIN_LAYER_FRONT] = true;

	// Mark chunk as initialized
	LOG("Chunk [%i, %i] generated", m_x, m_y);
}

void TerrainChunk::generateVertexBuffer(ChunkLoader *chunkLoader, TerrainLayer z)
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

	// Load all vertex data
	BlockID block;
	BlockID adjacentBlocks[8];

	// Setup counters
	uint vertexCount = 0, indexCount = 0;

	for(int y = 0; y < CHUNK_BLOCKS; ++y)
	{
		for(int x = 0; x < CHUNK_BLOCKS; ++x)
		{
			block = m_blocks[BLOCK_INDEX(x , y, z)];

			if(x == 0)
			{
				if(y == 0) adjacentBlocks[0] = m_adjacentChunks[0]->m_blocks[BLOCK_INDEX(CHUNK_BLOCKS-1, CHUNK_BLOCKS-1, z)];
				else       adjacentBlocks[0] = m_adjacentChunks[7]->m_blocks[BLOCK_INDEX(CHUNK_BLOCKS-1, y-1, z)];
			}
			else if(y == 0)
			{
				adjacentBlocks[0] = m_adjacentChunks[1]->m_blocks[BLOCK_INDEX(x-1, CHUNK_BLOCKS-1, z)];
			}
			else
			{
				adjacentBlocks[0] = m_blocks[BLOCK_INDEX(x-1, y-1, z)];
			}
				
			adjacentBlocks[1] = (y != 0) ? m_blocks[BLOCK_INDEX(x, y-1, z)] : m_adjacentChunks[1]->m_blocks[BLOCK_INDEX(x, CHUNK_BLOCKS-1, z)];

			if(x == CHUNK_BLOCKS-1)
			{
				if(y == 0) adjacentBlocks[2] = m_adjacentChunks[2]->m_blocks[BLOCK_INDEX(0, CHUNK_BLOCKS-1, z)];
				else       adjacentBlocks[2] = m_adjacentChunks[3]->m_blocks[BLOCK_INDEX(0, y-1, z)];
			}
			else if(y == 0)
			{
				adjacentBlocks[2] = m_adjacentChunks[1]->m_blocks[BLOCK_INDEX(x+1, CHUNK_BLOCKS-1, z)];
			}
			else
			{
				adjacentBlocks[2] = m_blocks[BLOCK_INDEX(x+1, y-1, z)];
			}

			adjacentBlocks[3] = (x != CHUNK_BLOCKS-1) ? m_blocks[BLOCK_INDEX(x+1, y, z)] : m_adjacentChunks[3]->m_blocks[BLOCK_INDEX(0, y, z)];
				
			if(x == CHUNK_BLOCKS-1)
			{
				if(y == CHUNK_BLOCKS-1) adjacentBlocks[4] = m_adjacentChunks[4]->m_blocks[BLOCK_INDEX(0, 0, z)];
				else                    adjacentBlocks[4] = m_adjacentChunks[3]->m_blocks[BLOCK_INDEX(0, y+1, z)];
			}
			else if(y == CHUNK_BLOCKS-1)
			{
				adjacentBlocks[4] = m_adjacentChunks[5]->m_blocks[BLOCK_INDEX(x+1, 0, z)];
			}
			else
			{
				adjacentBlocks[4] = m_blocks[BLOCK_INDEX(x+1, y+1, z)];
			}

			adjacentBlocks[5] = (y != CHUNK_BLOCKS-1) ? m_blocks[BLOCK_INDEX(x, y+1, z)] : m_adjacentChunks[5]->m_blocks[BLOCK_INDEX(x, 0, z)];
				
			if(x == 0)
			{
				if(y == CHUNK_BLOCKS-1) adjacentBlocks[6] = m_adjacentChunks[6]->m_blocks[BLOCK_INDEX(CHUNK_BLOCKS-1, 0, z)];
				else                    adjacentBlocks[6] = m_adjacentChunks[7]->m_blocks[BLOCK_INDEX(CHUNK_BLOCKS-1, y+1, z)];
			}
			else if(y == CHUNK_BLOCKS-1)
			{
				adjacentBlocks[6] = m_adjacentChunks[5]->m_blocks[BLOCK_INDEX(x-1, 0, z)];
			}
			else
			{
				adjacentBlocks[6] = m_blocks[BLOCK_INDEX(x-1, y+1, z)];
			}

			adjacentBlocks[7] = (x != 0) ? m_blocks[BLOCK_INDEX(x-1, y, z)] : m_adjacentChunks[7]->m_blocks[BLOCK_INDEX(CHUNK_BLOCKS-1, y, z)];

			if(block > BLOCK_ENTITY)
			{
				m_tmpQuads.push_back(BlockQuad(block, BLOCK_X0, BLOCK_Y0, BLOCK_X4, BLOCK_Y4, BLOCK_U1, BLOCK_V1, BLOCK_U5, BLOCK_V5));
				push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
			}
	
			// Bottom-right outer-corner
			if(adjacentBlocks[0] > BLOCK_ENTITY)
			{
				if(adjacentBlocks[0] != block && adjacentBlocks[0] != adjacentBlocks[1] && adjacentBlocks[0] != adjacentBlocks[7])
				{
					m_tmpQuads.push_back(BlockQuad(adjacentBlocks[0], BLOCK_X0, BLOCK_Y0, BLOCK_X1, BLOCK_Y1, BLOCK_U5, BLOCK_V0, BLOCK_U6, BLOCK_V1));
					push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
				}
			}

			// Draw bottom edge
			if(adjacentBlocks[1] > BLOCK_ENTITY)
			{
				if(adjacentBlocks[1] != block)
				{
					if(adjacentBlocks[1] == adjacentBlocks[3])
					{
						if(adjacentBlocks[1] == adjacentBlocks[7])
						{
							m_tmpQuads.push_back(BlockQuad(adjacentBlocks[1], BLOCK_X2, BLOCK_Y0, BLOCK_X4, BLOCK_Y2, BLOCK_U2, BLOCK_V7, BLOCK_U4, BLOCK_V8));
							push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
							m_tmpQuads.push_back(BlockQuad(adjacentBlocks[1], BLOCK_X0, BLOCK_Y0, BLOCK_X2, BLOCK_Y2, BLOCK_U0, BLOCK_V7, BLOCK_U2, BLOCK_V8));
							push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
						}
						else
						{
							m_tmpQuads.push_back(BlockQuad(adjacentBlocks[1], BLOCK_X0, BLOCK_Y0, BLOCK_X2, BLOCK_Y1, BLOCK_U1, BLOCK_V0, BLOCK_U3, BLOCK_V1));
							push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
							m_tmpQuads.push_back(BlockQuad(adjacentBlocks[1], BLOCK_X2, BLOCK_Y0, BLOCK_X4, BLOCK_Y2, BLOCK_U2, BLOCK_V7, BLOCK_U4, BLOCK_V8));
							push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
						}
					}
					else if(adjacentBlocks[1] == adjacentBlocks[7])
					{
						m_tmpQuads.push_back(BlockQuad(adjacentBlocks[1], BLOCK_X2, BLOCK_Y0, BLOCK_X4, BLOCK_Y1, BLOCK_U3, BLOCK_V0, BLOCK_U5, BLOCK_V1));
						push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
						m_tmpQuads.push_back(BlockQuad(adjacentBlocks[1], BLOCK_X0, BLOCK_Y0, BLOCK_X2, BLOCK_Y2, BLOCK_U0, BLOCK_V7, BLOCK_U2, BLOCK_V8));
						push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
					}
					else
					{
						m_tmpQuads.push_back(BlockQuad(adjacentBlocks[1], BLOCK_X0, BLOCK_Y0, BLOCK_X4, BLOCK_Y1, BLOCK_U1, BLOCK_V0, BLOCK_U5, BLOCK_V1));
						push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
					}
				}
			}

			// Bottom-left outer-corner
			if(adjacentBlocks[2] > BLOCK_ENTITY)
			{
				if(adjacentBlocks[2] != block && adjacentBlocks[2] != adjacentBlocks[1] && adjacentBlocks[2] != adjacentBlocks[3])
				{
					m_tmpQuads.push_back(BlockQuad(adjacentBlocks[2], BLOCK_X3, BLOCK_Y0, BLOCK_X4, BLOCK_Y1, BLOCK_U0, BLOCK_V0, BLOCK_U1, BLOCK_V1));
					push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
				}
			}

			// Draw left edge
			if(adjacentBlocks[3] > BLOCK_ENTITY)
			{
				if(adjacentBlocks[3] != block)
				{
					if(adjacentBlocks[3] == adjacentBlocks[1])
					{
						if(adjacentBlocks[3] != adjacentBlocks[5])
						{
							m_tmpQuads.push_back(BlockQuad(adjacentBlocks[3], BLOCK_X3, BLOCK_Y2, BLOCK_X4, BLOCK_Y4, BLOCK_U0, BLOCK_V1, BLOCK_U1, BLOCK_V3));
							push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
						}
					}
					else if(adjacentBlocks[3] == adjacentBlocks[5])
					{
						m_tmpQuads.push_back(BlockQuad(adjacentBlocks[3], BLOCK_X3, BLOCK_Y0, BLOCK_X4, BLOCK_Y2, BLOCK_U0, BLOCK_V3, BLOCK_U1, BLOCK_V5));
						push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
					}
					else
					{
						m_tmpQuads.push_back(BlockQuad(adjacentBlocks[3], BLOCK_X3, BLOCK_Y0, BLOCK_X4, BLOCK_Y4, BLOCK_U0, BLOCK_V1, BLOCK_U1, BLOCK_V5));
						push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
					}
				}
			}

			// Top-left outer-corner
			if(adjacentBlocks[4] > BLOCK_ENTITY)
			{
				if(adjacentBlocks[4] != block && adjacentBlocks[4] != adjacentBlocks[3] && adjacentBlocks[4] != adjacentBlocks[5])
				{
					m_tmpQuads.push_back(BlockQuad(adjacentBlocks[4], BLOCK_X3, BLOCK_Y3, BLOCK_X4, BLOCK_Y4, BLOCK_U0, BLOCK_V5, BLOCK_U1, BLOCK_V6));
					push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
				}
			}

			// Draw top edge
			if(adjacentBlocks[5] > BLOCK_ENTITY)
			{
				if(adjacentBlocks[5] != block)
				{
					if(adjacentBlocks[5] == adjacentBlocks[3])
					{
						if(adjacentBlocks[5] == adjacentBlocks[7])
						{
							m_tmpQuads.push_back(BlockQuad(adjacentBlocks[5], BLOCK_X2, BLOCK_Y2, BLOCK_X4, BLOCK_Y4, BLOCK_U2, BLOCK_V6, BLOCK_U4, BLOCK_V7));
							push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
							m_tmpQuads.push_back(BlockQuad(adjacentBlocks[5], BLOCK_X0, BLOCK_Y2, BLOCK_X2, BLOCK_Y4, BLOCK_U0, BLOCK_V6, BLOCK_U2, BLOCK_V7));
							push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
						}
						else
						{
							m_tmpQuads.push_back(BlockQuad(adjacentBlocks[5], BLOCK_X0, BLOCK_Y3, BLOCK_X2, BLOCK_Y4, BLOCK_U1, BLOCK_V5, BLOCK_U3, BLOCK_V6));
							push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
							m_tmpQuads.push_back(BlockQuad(adjacentBlocks[5], BLOCK_X2, BLOCK_Y2, BLOCK_X4, BLOCK_Y4, BLOCK_U2, BLOCK_V6, BLOCK_U4, BLOCK_V7));
							push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
						}
					}
					else if(adjacentBlocks[5] == adjacentBlocks[7])
					{
						m_tmpQuads.push_back(BlockQuad(adjacentBlocks[5], BLOCK_X2, BLOCK_Y3, BLOCK_X4, BLOCK_Y4, BLOCK_U3, BLOCK_V5, BLOCK_U5, BLOCK_V6));
						push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
						m_tmpQuads.push_back(BlockQuad(adjacentBlocks[5], BLOCK_X0, BLOCK_Y2, BLOCK_X2, BLOCK_Y4, BLOCK_U0, BLOCK_V6, BLOCK_U2, BLOCK_V7));
						push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
					}
					else
					{
						m_tmpQuads.push_back(BlockQuad(adjacentBlocks[5], BLOCK_X0, BLOCK_Y3, BLOCK_X4, BLOCK_Y4, BLOCK_U1, BLOCK_V5, BLOCK_U5, BLOCK_V6));
						push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
					}
				}
			}
	
			// Top-right outer-corner
			if(adjacentBlocks[6] > BLOCK_ENTITY)
			{
				if(adjacentBlocks[6] != block && adjacentBlocks[6] != adjacentBlocks[7] && adjacentBlocks[6] != adjacentBlocks[5])
				{
					m_tmpQuads.push_back(BlockQuad(adjacentBlocks[6], BLOCK_X0, BLOCK_Y3, BLOCK_X1, BLOCK_Y4, BLOCK_U5, BLOCK_V5, BLOCK_U6, BLOCK_V6));
					push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
				}
			}

			// Draw right edge
			if(adjacentBlocks[7] > BLOCK_ENTITY)
			{
				if(adjacentBlocks[7] != block)
				{
					if(adjacentBlocks[7] == adjacentBlocks[1])
					{
						if(adjacentBlocks[7] != adjacentBlocks[5])
						{
							m_tmpQuads.push_back(BlockQuad(adjacentBlocks[7], BLOCK_X0, BLOCK_Y2, BLOCK_X1, BLOCK_Y4, BLOCK_U5, BLOCK_V1, BLOCK_U6, BLOCK_V3));
							push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
						}
					}
					else if(adjacentBlocks[7] == adjacentBlocks[5])
					{
						m_tmpQuads.push_back(BlockQuad(adjacentBlocks[7], BLOCK_X0, BLOCK_Y0, BLOCK_X1, BLOCK_Y2, BLOCK_U5, BLOCK_V3, BLOCK_U6, BLOCK_V5));
						push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
					}
					else
					{
						m_tmpQuads.push_back(BlockQuad(adjacentBlocks[7], BLOCK_X0, BLOCK_Y0, BLOCK_X1, BLOCK_Y4, BLOCK_U5, BLOCK_V1, BLOCK_U6, BLOCK_V5));
						push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
					}
				}
			}

			while(!m_tmpQuads.empty())
			{
				BlockQuad &quad = m_tmpQuads[0];

				s_vertices[0 + vertexCount].set4f(VERTEX_POSITION, x + quad.x0, y + quad.y0);
				s_vertices[1 + vertexCount].set4f(VERTEX_POSITION, x + quad.x1, y + quad.y0);
				s_vertices[2 + vertexCount].set4f(VERTEX_POSITION, x + quad.x1, y + quad.y1);
				s_vertices[3 + vertexCount].set4f(VERTEX_POSITION, x + quad.x0, y + quad.y1);
				TextureRegion region = BlockData::getBlockAtlas()->get(quad.block, quad.u0, quad.v0, quad.u1, quad.v1);
				s_vertices[0 + vertexCount].set4f(VERTEX_TEX_COORD, region.uv0.x, region.uv1.y);
				s_vertices[1 + vertexCount].set4f(VERTEX_TEX_COORD, region.uv1.x, region.uv1.y);
				s_vertices[2 + vertexCount].set4f(VERTEX_TEX_COORD, region.uv1.x, region.uv0.y);
				s_vertices[3 + vertexCount].set4f(VERTEX_TEX_COORD, region.uv0.x, region.uv0.y);
		
				s_indices[0 + indexCount] = QUAD_INDICES[0] + vertexCount;
				s_indices[1 + indexCount] = QUAD_INDICES[1] + vertexCount;
				s_indices[2 + indexCount] = QUAD_INDICES[2] + vertexCount;
				s_indices[3 + indexCount] = QUAD_INDICES[3] + vertexCount;
				s_indices[4 + indexCount] = QUAD_INDICES[4] + vertexCount;
				s_indices[5 + indexCount] = QUAD_INDICES[5] + vertexCount;

				vertexCount += 4;
				indexCount += 6;
					
				pop_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
				m_tmpQuads.pop_back();  
			}

			m_tmpQuads.clear();
		}
	}
		
	// Set static vertex buffer data
	m_vertexBuffers[z].setData(s_vertices, vertexCount);
	m_indexBuffers[z].setData(s_indices, indexCount);

	// Not dirty
	m_dirty[z] = false;
}

// SERIALIZATION
void TerrainChunk::serialize(FileWriter &ss)
{
	/*LOG("Saving chunk [%i, %i]...", m_x, m_y);
		
	// Write chunk pos
	ss << m_x << endl;
	ss << m_y << endl;
		
	// Write blocks to stream
	for(int i = 0; i < TERRAIN_LAYER_COUNT; ++i)
	{
		for(int y = 0; y < CHUNK_BLOCKS; ++y)
		{
			for(int x = 0; x < CHUNK_BLOCKS; ++x)
			{
				Block block = m_blocks[BLOCK_INDEX(x, y, i)];
				ss << block.id << endl;
			}
		}
	}*/
}
	
void TerrainChunk::deserialize(stringstream &ss)
{
	// Initialize chunk
	/*int chunkX, chunkY;
	ss >> chunkX;
	ss >> chunkY;
		
	LOG("Loading chunk [%i, %i]...", chunkX, chunkY);
		
	load(chunkX, chunkY);
		
	// Load blocks from stream
	for(int i = 0; i < TERRAIN_LAYER_COUNT; ++i)
	{
		for(int y = 0; y < CHUNK_BLOCKS; ++y)
		{
			for(int x = 0; x < CHUNK_BLOCKS; ++x)
			{
				int block;
				ss >> block;
				m_blocks[BLOCK_INDEX(x, y, i)] = BlockID(block);
			}
		}
	}
		
	m_state = CHUNK_INITIALIZED;
	generateVBO();*/
}

// BLOCKS
BlockID TerrainChunk::getBlockAt(const int x, const int y, TerrainLayer layer) const
{
	return m_blocks[BLOCK_INDEX(x, y, layer)];
}
	
bool TerrainChunk::isBlockAt(const int x, const int y, TerrainLayer layer) const
{
	return m_blocks[BLOCK_INDEX(x, y, layer)] != BLOCK_EMPTY;
}
	
bool TerrainChunk::isBlockOccupied(const int x, const int y, TerrainLayer layer) const
{
	return m_blocks[BLOCK_INDEX(x, y, layer)] >= BLOCK_ENTITY;
}
	
bool TerrainChunk::setBlockAt(const int x, const int y, const BlockID block, TerrainLayer layer)
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

		return true; // return true as something was changed
	}
	return false; // nothing changed
}

// DRAWING
void TerrainChunk::draw(GraphicsContext &gfxContext, const TerrainLayer layer)
{
	// Draw vertex buffers
	gfxContext.drawIndexedPrimitives(GraphicsContext::PRIMITIVE_TRIANGLES, &m_vertexBuffers[layer], &m_indexBuffers[layer]);
}