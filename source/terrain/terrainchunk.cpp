#include "terrainchunk.h"
#include "terrain.h"
#include "constants.h"

#include "game/world.h"
#include "game/blockdata.h"

#include "generator/terraingen.h"

#define BLOCK_INDEX(x, y, z) (x+1) + (CHUNK_BLOCKS+2) * ((y+1) + (CHUNK_BLOCKS+2) * (z))

TerrainChunk::Block TerrainChunk::s_tempBlock;

TerrainChunk::Block::Block() :
	id(BLOCK_EMPTY)
{
	for(uint i = 0; i < 8; ++i)
	{
		next[i] = &s_tempBlock;
	}
}

TerrainChunk::Block::Block(BlockID id) :
	id(id)
{
	for(uint i = 0; i < 8; ++i)
	{
		next[i] = &s_tempBlock;
	}
}

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

TerrainChunk::TerrainChunk()
{
	// A dummy
	m_state = CHUNK_DUMMY;
	m_dirty = m_modified = false; // not modified
	m_shadowMap = xd::Texture2DPtr(new xd::Texture2D(xd::Pixmap(CHUNK_BLOCKS, CHUNK_BLOCKS)));

	// Initialize blocks
	m_blocks = new Block[CHUNK_BLOCKS*CHUNK_BLOCKS*TERRAIN_LAYER_COUNT];
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

	// Initialize neightbour chunks
	for(uint i = 0; i < 8; ++i)
	{
		m_nextChunk[i] = nullptr;
	}
}
	
// SERIALIZATION
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
				/*if(m_nextChunk[0])
				{
					if(y == -1)		m_blocks[BLOCK_INDEX(x, -1, z)] = m_nextChunk[0]->m_blocks[BLOCK_INDEX(x, CHUNK_BLOCKS-1, z)];
					else if(y == 0)	m_blocks[BLOCK_INDEX(x,  0, z)] = m_nextChunk[0]->m_blocks[BLOCK_INDEX(x, CHUNK_BLOCKS, z)];
				}

				if(m_nextChunk[1])
				{
					if(x == CHUNK_BLOCKS && y == -1)       m_blocks[BLOCK_INDEX(CHUNK_BLOCKS-1, 0, z)] = m_nextChunk[1]->m_blocks[BLOCK_INDEX(0, CHUNK_BLOCKS-1, z)];
					else if(x == CHUNK_BLOCKS-1 && y == 0) m_blocks[BLOCK_INDEX(CHUNK_BLOCKS-1, 0, z)] = m_nextChunk[1]->m_blocks[BLOCK_INDEX(-1, CHUNK_BLOCKS, z)];
				}

				if(m_nextChunk[2])
				{
					if(x == CHUNK_BLOCKS)											m_blocks[BLOCK_INDEX(CHUNK_BLOCKS, y, z)]					= m_nextChunk[2]->m_blocks[BLOCK_INDEX(0, y, z)];
					else if(x == CHUNK_BLOCKS-1)									m_blocks[BLOCK_INDEX(CHUNK_BLOCKS-1, y, z)]					= m_nextChunk[2]->m_blocks[BLOCK_INDEX(-1, y, z)];
				}

				if(m_nextChunk[3])
				{
					if(x == CHUNK_BLOCKS && y == CHUNK_BLOCKS)						m_blocks[BLOCK_INDEX(CHUNK_BLOCKS, CHUNK_BLOCKS, z)]		= m_nextChunk[3]->m_blocks[BLOCK_INDEX(0, 0, z)];
					else if(x == CHUNK_BLOCKS-1 && y == CHUNK_BLOCKS-1)				m_blocks[BLOCK_INDEX(CHUNK_BLOCKS-1, CHUNK_BLOCKS-1, z)]	= m_nextChunk[3]->m_blocks[BLOCK_INDEX(-1, -1, z)];
				}

				if(m_nextChunk[4])
				{
					if(y == CHUNK_BLOCKS)											m_blocks[BLOCK_INDEX(x, CHUNK_BLOCKS, z)]					= m_nextChunk[4]->m_blocks[BLOCK_INDEX(x, 0, z)];
					else if(y == CHUNK_BLOCKS-1)									m_blocks[BLOCK_INDEX(x, CHUNK_BLOCKS-1, z)]					= m_nextChunk[4]->m_blocks[BLOCK_INDEX(x, -1, z)];
				}

				if(m_nextChunk[5])
				{
					if(x == -1 && y == CHUNK_BLOCKS)								m_blocks[BLOCK_INDEX(-1, CHUNK_BLOCKS, z)]					= m_nextChunk[5]->m_blocks[BLOCK_INDEX(CHUNK_BLOCKS-1, 0, z)];
					else if(x == 0 && y == CHUNK_BLOCKS-1)							m_blocks[BLOCK_INDEX(0, CHUNK_BLOCKS-1, z)]					= m_nextChunk[5]->m_blocks[BLOCK_INDEX(CHUNK_BLOCKS, -1, z)];
				}

				if(m_nextChunk[6])
				{
					if(x == -1)														m_blocks[BLOCK_INDEX(-1, y, z)]								= m_nextChunk[6]->m_blocks[BLOCK_INDEX(CHUNK_BLOCKS-1, y, z)];
					else if(x == 0)													m_blocks[BLOCK_INDEX(0, y, z)]								= m_nextChunk[6]->m_blocks[BLOCK_INDEX(CHUNK_BLOCKS, y, z)];
				}

				if(m_nextChunk[7])
				{
					if(x == -1 && y == -1)											m_blocks[BLOCK_INDEX(-1, -1, z)]							= m_nextChunk[7]->m_blocks[BLOCK_INDEX(CHUNK_BLOCKS-1,  CHUNK_BLOCKS-1, z)];
					else if(x == 0 && y == 0)										m_blocks[BLOCK_INDEX(0, 0, z)]								= m_nextChunk[7]->m_blocks[BLOCK_INDEX(CHUNK_BLOCKS,  CHUNK_BLOCKS, z)];
				}*/

				// Get block from TerrainGen
				m_blocks[BLOCK_INDEX(x, y, z)] = TerrainGen::getBlockAt(m_x * CHUNK_BLOCKS + x, m_y * CHUNK_BLOCKS + y, (TerrainLayer)z);
			}
		}
	}
		
	for(int z = 0; z < TERRAIN_LAYER_COUNT; ++z)
	{
		for(int y = 0; y < CHUNK_BLOCKS; ++y)
		{
			for(int x = 0; x < CHUNK_BLOCKS; ++x)
			{
				Block &block = m_blocks[BLOCK_INDEX(x, y, z)]; // NOTE TO SELF: There might exists some magic formula which will give me a pointer offset
					                                           // that i can simply apply to the current block adress instead of having to calculate the BLOCK_INDEX each time
				if(x != 0 && y != 0)							block.next[0] = &m_blocks[BLOCK_INDEX(x-1, y-1, z)];
				if(y != 0)										block.next[1] = &m_blocks[BLOCK_INDEX(x,   y-1, z)];
				if(x != CHUNK_BLOCKS-1 && y != 0)				block.next[2] = &m_blocks[BLOCK_INDEX(x+1, y-1, z)];
				if(x != CHUNK_BLOCKS-1)							block.next[3] = &m_blocks[BLOCK_INDEX(x+1, y,   z)];
				if(x != CHUNK_BLOCKS-1 && y != CHUNK_BLOCKS-1)	block.next[4] = &m_blocks[BLOCK_INDEX(x+1, y+1, z)];
				if(y != CHUNK_BLOCKS-1)							block.next[5] = &m_blocks[BLOCK_INDEX(x,   y+1, z)];
				if(x != 0 && y != CHUNK_BLOCKS-1)				block.next[6] = &m_blocks[BLOCK_INDEX(x-1, y+1, z)];
				if(x != 0)										block.next[7] = &m_blocks[BLOCK_INDEX(x-1, y  , z)];
			}
		}
	}

	// Load shadow map
	xd::Pixmap pixmap(CHUNK_BLOCKS, CHUNK_BLOCKS);
	for(int y = 0; y < CHUNK_BLOCKS; ++y)
	{
		for(int x = 0; x < CHUNK_BLOCKS; ++x)
		{
			float shadow = 1.0f;
			bool shadowCaster = m_blocks[BLOCK_INDEX(x, y, TERRAIN_LAYER_MIDDLE)].id <= BLOCK_OCCUPIED;
			for(uint i = 0; i < TERRAIN_LAYER_COUNT; ++i)
			{
				shadow -= BlockData::get(m_blocks[BLOCK_INDEX(x, y, i)].id).getOpacity();
			}
			pixmap.setColor(x, CHUNK_BLOCKS - y - 1, xd::Color(255 * max(shadow, 0.0f), 255, 255, 255 * shadowCaster)); // rgb = light value, a = shadow casting value
		}
	}
	m_shadowMap->updatePixmap(pixmap);
		
	// Mark as dirty
	m_dirty = true;

	// Mark chunk as initialized
	xd::LOG("Chunk [%i, %i] generated", m_x, m_y);
}

// Block texture coordinates
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

xd::Vertex *TerrainChunk::s_vertices = nullptr;
uint *TerrainChunk::s_indices = new uint[6*12*16*16*3];
void TerrainChunk::generateVBO()
{
	// Allocate vertices
	uint vertexCount = 0, indexCount = 0;

	// Load all vertex data
	for(int y = 0; y < CHUNK_BLOCKS; ++y)
	{
		for(int x = 0; x < CHUNK_BLOCKS; ++x)
		{
			for(int i = 0; i < TERRAIN_LAYER_COUNT; ++i)
			{
				Block &block = m_blocks[BLOCK_INDEX(x, y, i)];

				//uint offset = 0;
				if(block.id > BLOCK_OCCUPIED)
				{
					m_tmpQuads.push_back(BlockQuad(block.id, BLOCK_X0, BLOCK_Y0, BLOCK_X4, BLOCK_Y4, BLOCK_U1, BLOCK_V1, BLOCK_U5, BLOCK_V5));
					push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
				}
	
				// Bottom-right outer-corner
				if(block.next[0]->id > BLOCK_OCCUPIED)
				{
					if(block.next[0]->id != block.id && block.next[0]->id != block.next[1]->id && block.next[0]->id != block.next[7]->id)
					{
						m_tmpQuads.push_back(BlockQuad(block.next[0]->id, BLOCK_X0, BLOCK_Y0, BLOCK_X1, BLOCK_Y1, BLOCK_U5, BLOCK_V0, BLOCK_U6, BLOCK_V1));
						push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
					}
				}

				// Draw bottom edge
				if(block.next[1]->id > BLOCK_OCCUPIED)
				{
					if(block.next[1]->id != block.id)
					{
						if(block.next[1]->id == block.next[3]->id)
						{
							if(block.next[1]->id == block.next[7]->id)
							{
								m_tmpQuads.push_back(BlockQuad(block.next[1]->id, BLOCK_X2, BLOCK_Y0, BLOCK_X4, BLOCK_Y2, BLOCK_U2, BLOCK_V7, BLOCK_U4, BLOCK_V8));
								push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
								m_tmpQuads.push_back(BlockQuad(block.next[1]->id, BLOCK_X0, BLOCK_Y0, BLOCK_X2, BLOCK_Y2, BLOCK_U0, BLOCK_V7, BLOCK_U2, BLOCK_V8));
								push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
							}
							else
							{
								m_tmpQuads.push_back(BlockQuad(block.next[1]->id, BLOCK_X0, BLOCK_Y0, BLOCK_X2, BLOCK_Y1, BLOCK_U1, BLOCK_V0, BLOCK_U3, BLOCK_V1));
								push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
								m_tmpQuads.push_back(BlockQuad(block.next[1]->id, BLOCK_X2, BLOCK_Y0, BLOCK_X4, BLOCK_Y2, BLOCK_U2, BLOCK_V7, BLOCK_U4, BLOCK_V8));
								push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
							}
						}
						else if(block.next[1]->id == block.next[7]->id)
						{
							m_tmpQuads.push_back(BlockQuad(block.next[1]->id, BLOCK_X2, BLOCK_Y0, BLOCK_X4, BLOCK_Y1, BLOCK_U3, BLOCK_V0, BLOCK_U5, BLOCK_V1));
							push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
							m_tmpQuads.push_back(BlockQuad(block.next[1]->id, BLOCK_X0, BLOCK_Y0, BLOCK_X2, BLOCK_Y2, BLOCK_U0, BLOCK_V7, BLOCK_U2, BLOCK_V8));
							push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
						}
						else
						{
							m_tmpQuads.push_back(BlockQuad(block.next[1]->id, BLOCK_X0, BLOCK_Y0, BLOCK_X4, BLOCK_Y1, BLOCK_U1, BLOCK_V0, BLOCK_U5, BLOCK_V1));
							push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
						}
					}
				}

				// Bottom-left outer-corner
				if(block.next[2]->id > BLOCK_OCCUPIED)
				{
					if(block.next[2]->id != block.id && block.next[2]->id != block.next[1]->id && block.next[2]->id != block.next[3]->id)
					{
						m_tmpQuads.push_back(BlockQuad(block.next[2]->id, BLOCK_X3, BLOCK_Y0, BLOCK_X4, BLOCK_Y1, BLOCK_U0, BLOCK_V0, BLOCK_U1, BLOCK_V1));
						push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
					}
				}

				// Draw left edge
				if(block.next[3]->id > BLOCK_OCCUPIED)
				{
					if(block.next[3]->id != block.id)
					{
						if(block.next[3]->id == block.next[1]->id)
						{
							if(block.next[3]->id != block.next[5]->id)
							{
								m_tmpQuads.push_back(BlockQuad(block.next[3]->id, BLOCK_X3, BLOCK_Y2, BLOCK_X4, BLOCK_Y4, BLOCK_U0, BLOCK_V1, BLOCK_U1, BLOCK_V3));
								push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
							}
						}
						else if(block.next[3]->id == block.next[5]->id)
						{
							m_tmpQuads.push_back(BlockQuad(block.next[3]->id, BLOCK_X3, BLOCK_Y0, BLOCK_X4, BLOCK_Y2, BLOCK_U0, BLOCK_V3, BLOCK_U1, BLOCK_V5));
							push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
						}
						else
						{
							m_tmpQuads.push_back(BlockQuad(block.next[3]->id, BLOCK_X3, BLOCK_Y0, BLOCK_X4, BLOCK_Y4, BLOCK_U0, BLOCK_V1, BLOCK_U1, BLOCK_V5));
							push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
						}
					}
				}

				// Top-left outer-corner
				if(block.next[4]->id > BLOCK_OCCUPIED)
				{
					if(block.next[4]->id != block.id && block.next[4]->id != block.next[3]->id && block.next[4]->id != block.next[5]->id)
					{
						m_tmpQuads.push_back(BlockQuad(block.next[4]->id, BLOCK_X3, BLOCK_Y3, BLOCK_X4, BLOCK_Y4, BLOCK_U0, BLOCK_V5, BLOCK_U1, BLOCK_V6));
						push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
					}
				}

				// Draw top edge
				if(block.next[5]->id > BLOCK_OCCUPIED)
				{
					if(block.next[5]->id != block.id)
					{
						if(block.next[5]->id == block.next[3]->id)
						{
							if(block.next[5]->id == block.next[7]->id)
							{
								m_tmpQuads.push_back(BlockQuad(block.next[5]->id, BLOCK_X2, BLOCK_Y2, BLOCK_X4, BLOCK_Y4, BLOCK_U2, BLOCK_V6, BLOCK_U4, BLOCK_V7));
								push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
								m_tmpQuads.push_back(BlockQuad(block.next[5]->id, BLOCK_X0, BLOCK_Y2, BLOCK_X2, BLOCK_Y4, BLOCK_U0, BLOCK_V6, BLOCK_U2, BLOCK_V7));
								push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
							}
							else
							{
								m_tmpQuads.push_back(BlockQuad(block.next[5]->id, BLOCK_X0, BLOCK_Y3, BLOCK_X2, BLOCK_Y4, BLOCK_U1, BLOCK_V5, BLOCK_U3, BLOCK_V6));
								push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
								m_tmpQuads.push_back(BlockQuad(block.next[5]->id, BLOCK_X2, BLOCK_Y2, BLOCK_X4, BLOCK_Y4, BLOCK_U2, BLOCK_V6, BLOCK_U4, BLOCK_V7));
								push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
							}
						}
						else if(block.next[5]->id == block.next[7]->id)
						{
							m_tmpQuads.push_back(BlockQuad(block.next[5]->id, BLOCK_X2, BLOCK_Y3, BLOCK_X4, BLOCK_Y4, BLOCK_U3, BLOCK_V5, BLOCK_U5, BLOCK_V6));
							push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
							m_tmpQuads.push_back(BlockQuad(block.next[5]->id, BLOCK_X0, BLOCK_Y2, BLOCK_X2, BLOCK_Y4, BLOCK_U0, BLOCK_V6, BLOCK_U2, BLOCK_V7));
							push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
						}
						else
						{
							m_tmpQuads.push_back(BlockQuad(block.next[5]->id, BLOCK_X0, BLOCK_Y3, BLOCK_X4, BLOCK_Y4, BLOCK_U1, BLOCK_V5, BLOCK_U5, BLOCK_V6));
							push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
						}
					}
				}
	
				// Top-right outer-corner
				if(block.next[6]->id > BLOCK_OCCUPIED)
				{
					if(block.next[6]->id != block.id && block.next[6]->id != block.next[7]->id && block.next[6]->id != block.next[5]->id)
					{
						m_tmpQuads.push_back(BlockQuad(block.next[6]->id, BLOCK_X0, BLOCK_Y3, BLOCK_X1, BLOCK_Y4, BLOCK_U5, BLOCK_V5, BLOCK_U6, BLOCK_V6));
						push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
					}
				}

				// Draw right edge
				if(block.next[7]->id > BLOCK_OCCUPIED)
				{
					if(block.next[7]->id != block.id)
					{
						if(block.next[7]->id == block.next[1]->id)
						{
							if(block.next[7]->id != block.next[5]->id)
							{
								m_tmpQuads.push_back(BlockQuad(block.next[7]->id, BLOCK_X0, BLOCK_Y2, BLOCK_X1, BLOCK_Y4, BLOCK_U5, BLOCK_V1, BLOCK_U6, BLOCK_V3));
								push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
							}
						}
						else if(block.next[7]->id == block.next[5]->id)
						{
							m_tmpQuads.push_back(BlockQuad(block.next[7]->id, BLOCK_X0, BLOCK_Y0, BLOCK_X1, BLOCK_Y2, BLOCK_U5, BLOCK_V3, BLOCK_U6, BLOCK_V5));
							push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
						}
						else
						{
							m_tmpQuads.push_back(BlockQuad(block.next[7]->id, BLOCK_X0, BLOCK_Y0, BLOCK_X1, BLOCK_Y4, BLOCK_U5, BLOCK_V1, BLOCK_U6, BLOCK_V5));
							push_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
						}
					}
				}

				while(!m_tmpQuads.empty())
				{
					BlockQuad &quad = m_tmpQuads[0];

					s_vertices[0 + vertexCount].set4f(xd::VERTEX_POSITION, x + quad.x0, y + quad.y0);
					s_vertices[1 + vertexCount].set4f(xd::VERTEX_POSITION, x + quad.x1, y + quad.y0);
					s_vertices[2 + vertexCount].set4f(xd::VERTEX_POSITION, x + quad.x1, y + quad.y1);
					s_vertices[3 + vertexCount].set4f(xd::VERTEX_POSITION, x + quad.x0, y + quad.y1);
					xd::TextureRegion region = BlockData::getBlockAtlas()->get(quad.block, quad.u0, quad.v0, quad.u1, quad.v1);
					s_vertices[0 + vertexCount].set4f(xd::VERTEX_TEX_COORD, region.uv0.x, region.uv1.y);
					s_vertices[1 + vertexCount].set4f(xd::VERTEX_TEX_COORD, region.uv1.x, region.uv1.y);
					s_vertices[2 + vertexCount].set4f(xd::VERTEX_TEX_COORD, region.uv1.x, region.uv0.y);
					s_vertices[3 + vertexCount].set4f(xd::VERTEX_TEX_COORD, region.uv0.x, region.uv0.y);
		
					s_indices[0 + indexCount] = xd::QUAD_INDICES[0] + vertexCount;
					s_indices[1 + indexCount] = xd::QUAD_INDICES[1] + vertexCount;
					s_indices[2 + indexCount] = xd::QUAD_INDICES[2] + vertexCount;
					s_indices[3 + indexCount] = xd::QUAD_INDICES[3] + vertexCount;
					s_indices[4 + indexCount] = xd::QUAD_INDICES[4] + vertexCount;
					s_indices[5 + indexCount] = xd::QUAD_INDICES[5] + vertexCount;

					vertexCount += 4;
					indexCount += 6;
					
					pop_heap(m_tmpQuads.begin(), m_tmpQuads.end(), greater<BlockQuad>());
					m_tmpQuads.pop_back();  
				}

				m_tmpQuads.clear();
			}
		}
	}
	
	// Create static vbo
	m_vbo.setData(s_vertices, vertexCount);
	m_ibo.setData(s_indices, indexCount);

	// Not dirty
	m_dirty = false;
}

void TerrainChunk::serialize(xd::FileWriter &ss)
{
	xd::LOG("Saving chunk [%i, %i]...", m_x, m_y);
		
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
	}
}
	
void TerrainChunk::deserialize(stringstream &ss)
{
	// Initialize chunk
	int chunkX, chunkY;
	ss >> chunkX;
	ss >> chunkY;
		
	xd::LOG("Loading chunk [%i, %i]...", chunkX, chunkY);
		
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
	generateVBO();
}
	
ChunkState TerrainChunk::getState() const
{
	return m_state;
}
	
BlockID TerrainChunk::getBlockAt(const int x, const int y, TerrainLayer layer) const
{
	return m_state != CHUNK_DUMMY ? m_blocks[BLOCK_INDEX(x, y, layer)].id : BLOCK_EMPTY;
}
	
bool TerrainChunk::isBlockAt(const int x, const int y, TerrainLayer layer) const
{
	return m_state != CHUNK_DUMMY && m_blocks[BLOCK_INDEX(x, y, layer)].id != BLOCK_EMPTY;
}
	
bool TerrainChunk::isBlockOccupied(const int x, const int y, TerrainLayer layer) const
{
	return m_state != CHUNK_DUMMY && m_blocks[BLOCK_INDEX(x, y, layer)].id >= BLOCK_OCCUPIED;
}
	
bool TerrainChunk::setBlockAt(const int x, const int y, const BlockID block, TerrainLayer layer)
{
	// Make sure we can add a block here
	if(m_state == CHUNK_INITIALIZED && m_blocks[BLOCK_INDEX(x, y, layer)].id != block)
	{
		// Set the block value
		m_blocks[BLOCK_INDEX(x, y, layer)].id = block;
		m_dirty = m_modified = true; // mark chunk as modified
		if(m_nextChunk[0] && y == 0)										m_nextChunk[0]->m_dirty = true;
		if(m_nextChunk[1] && x == CHUNK_BLOCKS-1 && y == 0)					m_nextChunk[1]->m_dirty = true;
		if(m_nextChunk[2] && x == CHUNK_BLOCKS-1)							m_nextChunk[2]->m_dirty = true;
		if(m_nextChunk[3] && x == CHUNK_BLOCKS-1 && y == CHUNK_BLOCKS-1)	m_nextChunk[3]->m_dirty = true;
		if(m_nextChunk[4] && y == CHUNK_BLOCKS-1)							m_nextChunk[4]->m_dirty = true;
		if(m_nextChunk[5] && x == 0 && y == CHUNK_BLOCKS-1)					m_nextChunk[5]->m_dirty = true;
		if(m_nextChunk[6] && x == 0)										m_nextChunk[6]->m_dirty = true;
		if(m_nextChunk[7] && x == 0 && y == 0)								m_nextChunk[7]->m_dirty = true;
		
		// Update shadow map
		float shadow = 1.0f;
		bool shadowCaster = m_blocks[BLOCK_INDEX(x, y, TERRAIN_LAYER_MIDDLE)].id <= BLOCK_OCCUPIED;
		for(uint i = 0; i < TERRAIN_LAYER_COUNT; ++i)
		{
			shadow -= BlockData::get(m_blocks[BLOCK_INDEX(x, y, i)].id).getOpacity();
		}
		const uchar pixel[4] = { 255 * max(shadow, 0.0f), 255, 255, 255 * shadowCaster };
		m_shadowMap->updatePixmap(x, CHUNK_BLOCKS - y - 1, xd::Pixmap(1, 1, pixel));

		return true; // return true as something was changed
	}
	return false; // nothing changed
}

// DRAWING
void TerrainChunk::draw(xd::GraphicsContext &gfxContext)
{
	// If dirty generate VBOs
	if(m_dirty) generateVBO();
		
	// Draw blocks
	gfxContext.drawIndexedPrimitives(xd::GraphicsContext::PRIMITIVE_TRIANGLES, &m_vbo, &m_ibo);
}