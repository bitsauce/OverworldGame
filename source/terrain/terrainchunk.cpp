#include "terrainchunk.h"
#include "terrain.h"
#include "terraingen.h"
#include "constants.h"

#include "game/world.h"
#include "game/blockdata.h"

#include <Box2D/Box2D.h>

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
}
	
TerrainChunk::TerrainChunk(int chunkX, int chunkY)
{
	init(chunkX, chunkY);
}

#define BLOCK_INDEX(x, y, z) (x+1) + (CHUNK_BLOCKS+2) * ((y+1) + (CHUNK_BLOCKS+2) * (z))
#define FIXTURE_INDEX(x, y) (x) + CHUNK_BLOCKS * (y)
	
// SERIALIZATION
void TerrainChunk::init(int chunkX, int chunkY)
{
	// Set chunk vars
	m_state = CHUNK_GENERATING;
	m_x = chunkX;
	m_y = chunkY;
	m_nextChunksGenerated = m_dirty = m_modified = false; // not modified
	m_shadowRadius = 4;
	XPixmap pixmap(CHUNK_PX + m_shadowRadius*2, CHUNK_PX + m_shadowRadius*2); // NOTE TO SELF: Concider creating this at start up and each time the chunk size change (for example in an options menu)
	                                                                          // and reusing that object instead of creating a new XPixmap for every chunk.
	m_shadowMap   = new XTexture(pixmap);
	m_shadowPass1 = new XTexture(pixmap);
	m_shadowPass2 = new XTexture(pixmap);
	m_shadowPass2->setFiltering(XTexture::LINEAR);
	
	m_tmpQuads.resize(9);

	// Initialize blocks
	m_blocks = new Block*[(CHUNK_BLOCKS+2)*(CHUNK_BLOCKS+2)*TERRAIN_LAYER_COUNT];
	for(int z = 0; z < TERRAIN_LAYER_COUNT; ++z)
	{
		for(int y = -1; y < CHUNK_BLOCKS+1; ++y)
		{
			for(int x = -1; x < CHUNK_BLOCKS+1; ++x)
			{
				m_blocks[BLOCK_INDEX(x, y, z)] = nullptr;
			}
		}
	}

	// Initialize neightbour chunks
	for(uint i = 0; i < 8; ++i)
	{
		m_nextChunk[i] = nullptr;
	}

	// Create body
	b2BodyDef def;
	def.type = b2_staticBody;
	def.position.Set(chunkX * CHUNK_PXF, chunkY * CHUNK_PXF);
	def.allowSleep = true;
	
	m_body = World::getb2World()->CreateBody(&def);
	m_body->SetUserData(World::getTerrain());
		
	// Resize fixture grid
	m_fixtures = new b2Fixture*[CHUNK_BLOCKS*CHUNK_BLOCKS];
	for(uint y = 0; y < CHUNK_BLOCKS; ++y)
	{
		for(uint x = 0; x < CHUNK_BLOCKS; ++x)
		{
			m_fixtures[FIXTURE_INDEX(x, y)] = nullptr;
		}
	}
}
	
void TerrainChunk::generate()
{
	if(m_state == CHUNK_GENERATING)
	{
		// Set all blocks
		for(int z = 0; z < TERRAIN_LAYER_COUNT; ++z)
		{
			for(int y = -1; y < CHUNK_BLOCKS+1; ++y)
			{
				for(int x = -1; x < CHUNK_BLOCKS+1; ++x)
				{
					if(m_nextChunk[0])
					{
						if(y == -1)														m_blocks[BLOCK_INDEX(x, -1, z)]								= m_nextChunk[0]->m_blocks[BLOCK_INDEX(x, CHUNK_BLOCKS-1, z)];
						else if(y == 0)													m_blocks[BLOCK_INDEX(x, 0, z)]								= m_nextChunk[0]->m_blocks[BLOCK_INDEX(x, CHUNK_BLOCKS, z)];
					}

					if(m_nextChunk[1])
					{
						if(x == CHUNK_BLOCKS && y == -1)								m_blocks[BLOCK_INDEX(CHUNK_BLOCKS-1, 0, z)]					= m_nextChunk[1]->m_blocks[BLOCK_INDEX(0, CHUNK_BLOCKS-1, z)];
						else if(x == CHUNK_BLOCKS-1 && y == 0)							m_blocks[BLOCK_INDEX(CHUNK_BLOCKS-1, 0, z)]					= m_nextChunk[1]->m_blocks[BLOCK_INDEX(-1, CHUNK_BLOCKS, z)];
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
					}

					if(m_blocks[BLOCK_INDEX(x, y, z)] == nullptr)
					{
						// Block doesn't exists yet, get block from TerrainGen
						m_blocks[BLOCK_INDEX(x, y, z)] = new Block(TerrainGen::getBlockAt(m_x * CHUNK_BLOCKS + x, m_y * CHUNK_BLOCKS + y, (TerrainLayer)z));
					}
				}
			}
		}
		
		for(int z = 0; z < TERRAIN_LAYER_COUNT; ++z)
		{
			for(int y = 0; y < CHUNK_BLOCKS; ++y)
			{
				for(int x = 0; x < CHUNK_BLOCKS; ++x)
				{
					Block *block = m_blocks[BLOCK_INDEX(x, y, z)]; // NOTE TO SELF: There might exists some magic formula which will give me a pointer offset
					                                               // that i can simply apply to the current block adress instead of having to calculate the BLOCK_INDEX each time
					block->next[0] = m_blocks[BLOCK_INDEX(x-1, y-1, z)];
					block->next[1] = m_blocks[BLOCK_INDEX(x,   y-1, z)];
					block->next[2] = m_blocks[BLOCK_INDEX(x+1, y-1, z)];
					block->next[3] = m_blocks[BLOCK_INDEX(x+1, y,   z)];
					block->next[4] = m_blocks[BLOCK_INDEX(x+1, y+1, z)];
					block->next[5] = m_blocks[BLOCK_INDEX(x,   y+1, z)];
					block->next[6] = m_blocks[BLOCK_INDEX(x-1, y+1, z)];
					block->next[7] = m_blocks[BLOCK_INDEX(x-1, y  , z)];
				}
			}
		}
		
		// Mark as dirty
		m_dirty = true;
			
		// Re-generate neightbouring chunks
		if(m_nextChunk[0]) m_nextChunk[0]->m_dirty = true;
		if(m_nextChunk[1]) m_nextChunk[1]->m_dirty = true;
		if(m_nextChunk[2]) m_nextChunk[2]->m_dirty = true;
		if(m_nextChunk[3]) m_nextChunk[3]->m_dirty = true;
		if(m_nextChunk[4]) m_nextChunk[4]->m_dirty = true;
		if(m_nextChunk[5]) m_nextChunk[5]->m_dirty = true;
		if(m_nextChunk[6]) m_nextChunk[6]->m_dirty = true;
		if(m_nextChunk[7]) m_nextChunk[7]->m_dirty = true;
			
		// Mark chunk as initialized
		m_state = CHUNK_INITIALIZED;
		LOG("Chunk [%i, %i] generated", m_x, m_y);
	}
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
#define BLOCK_V0 (0.0f / 8.0f)
#define BLOCK_U1 (1.0f / 6.0f)
#define BLOCK_V1 (1.0f / 8.0f)
#define BLOCK_U2 (2.0f / 6.0f)
#define BLOCK_V2 (2.0f / 8.0f)
#define BLOCK_U3 (3.0f / 6.0f)
#define BLOCK_V3 (3.0f / 8.0f)
#define BLOCK_U4 (4.0f / 6.0f)
#define BLOCK_V4 (4.0f / 8.0f)
#define BLOCK_U5 (5.0f / 6.0f)
#define BLOCK_V5 (5.0f / 8.0f)
#define BLOCK_U6 (6.0f / 6.0f)
#define BLOCK_V6 (6.0f / 8.0f)
#define BLOCK_V7 (7.0f / 8.0f)
#define BLOCK_V8 (8.0f / 8.0f)

void TerrainChunk::generateVBO()
{
	// Create new vbo
	m_vbo = shared_ptr<XVertexBuffer>(new XVertexBuffer(World::getTerrain()->getVertexFormat()));
		
	// Load all vertex data
	for(int y = 0; y < CHUNK_BLOCKS; ++y)
	{
		for(int x = 0; x < CHUNK_BLOCKS; ++x)
		{
			for(int i = TERRAIN_LAYER_COUNT-1; i >= 0; --i)
			{
				Block *block = m_blocks[BLOCK_INDEX(x, y, i)];

				XVertexFormat &format = World::getTerrain()->getVertexFormat();

				uint offset = 0;
				if(block->id != BLOCK_EMPTY)
				{
					m_tmpQuads[offset++] = BlockQuad(block->id, BLOCK_X0, BLOCK_Y0, BLOCK_X4, BLOCK_Y4, BLOCK_U1, BLOCK_V1, BLOCK_U5, BLOCK_V5);
				}
	
				// Bottom-right outer-corner
				if(block->next[0]->id != BLOCK_EMPTY)
				{
					if(block->next[0]->id != block->id && block->next[0]->id != block->next[1]->id && block->next[0]->id != block->next[7]->id)
					{
						m_tmpQuads[offset++] = BlockQuad(block->next[0]->id, BLOCK_X0, BLOCK_Y0, BLOCK_X1, BLOCK_Y1, BLOCK_U5, BLOCK_V0, BLOCK_U6, BLOCK_V1);
					}
				}

				// Draw bottom edge
				if(block->next[1]->id != BLOCK_EMPTY)
				{
					if(block->next[1]->id != block->id)
					{
						if(block->next[1]->id == block->next[3]->id)
						{
							if(block->next[1]->id == block->next[7]->id)
							{
								m_tmpQuads[offset++] = BlockQuad(block->next[1]->id, BLOCK_X1, BLOCK_Y0, BLOCK_X3, BLOCK_Y1, BLOCK_U2, BLOCK_V0, BLOCK_U4, BLOCK_V1);
								m_tmpQuads[offset++] = BlockQuad(block->next[1]->id, BLOCK_X3, BLOCK_Y0, BLOCK_X4, BLOCK_Y1, BLOCK_U1, BLOCK_V7, BLOCK_U2, BLOCK_V8);
								m_tmpQuads[offset++] = BlockQuad(block->next[1]->id, BLOCK_X0, BLOCK_Y0, BLOCK_X1, BLOCK_Y1, BLOCK_U0, BLOCK_V7, BLOCK_U1, BLOCK_V8);
							}
							else
							{
								m_tmpQuads[offset++] = BlockQuad(block->next[1]->id, BLOCK_X0, BLOCK_Y0, BLOCK_X3, BLOCK_Y1, BLOCK_U1, BLOCK_V0, BLOCK_U4, BLOCK_V1);
								m_tmpQuads[offset++] = BlockQuad(block->next[1]->id, BLOCK_X3, BLOCK_Y0, BLOCK_X4, BLOCK_Y1, BLOCK_U1, BLOCK_V7, BLOCK_U2, BLOCK_V8);
							}
						}
						else if(block->next[1]->id == block->next[7]->id)
						{
							m_tmpQuads[offset++] = BlockQuad(block->next[1]->id, BLOCK_X1, BLOCK_Y0, BLOCK_X4, BLOCK_Y1, BLOCK_U2, BLOCK_V0, BLOCK_U5, BLOCK_V1);
							m_tmpQuads[offset++] = BlockQuad(block->next[1]->id, BLOCK_X0, BLOCK_Y0, BLOCK_X1, BLOCK_Y1, BLOCK_U0, BLOCK_V7, BLOCK_U1, BLOCK_V8);
						}
						else
						{
							m_tmpQuads[offset++] = BlockQuad(block->next[1]->id, BLOCK_X0, BLOCK_Y0, BLOCK_X4, BLOCK_Y1, BLOCK_U1, BLOCK_V0, BLOCK_U5, BLOCK_V1);
						}
					}
				}

				// Bottom-left outer-corner
				if(block->next[2]->id != BLOCK_EMPTY)
				{
					if(block->next[2]->id != block->id && block->next[2]->id != block->next[1]->id && block->next[2]->id != block->next[3]->id)
					{
						m_tmpQuads[offset++] = BlockQuad(block->next[2]->id, BLOCK_X3, BLOCK_Y0, BLOCK_X4, BLOCK_Y1, BLOCK_U0, BLOCK_V0, BLOCK_U1, BLOCK_V1);
					}
				}

				// Draw left edge
				if(block->next[3]->id != BLOCK_EMPTY)
				{
					if(block->next[3]->id != block->id)
					{
						if(block->next[3]->id == block->next[1]->id)
						{
							if(block->next[3]->id == block->next[5]->id)
							{
								m_tmpQuads[offset++] = BlockQuad(block->next[3]->id, BLOCK_X3, BLOCK_Y1, BLOCK_X4, BLOCK_Y3, BLOCK_U0, BLOCK_V2, BLOCK_U1, BLOCK_V4);
							}
							else
							{
								m_tmpQuads[offset++] = BlockQuad(block->next[3]->id, BLOCK_X3, BLOCK_Y1, BLOCK_X4, BLOCK_Y4, BLOCK_U0, BLOCK_V2, BLOCK_U1, BLOCK_V5);
							}
						}
						else if(block->next[3]->id == block->next[5]->id)
						{
							m_tmpQuads[offset++] = BlockQuad(block->next[3]->id, BLOCK_X3, BLOCK_Y0, BLOCK_X4, BLOCK_Y3, BLOCK_U0, BLOCK_V2, BLOCK_U1, BLOCK_V5);
						}
						else
						{
							m_tmpQuads[offset++] = BlockQuad(block->next[3]->id, BLOCK_X3, BLOCK_Y0, BLOCK_X4, BLOCK_Y4, BLOCK_U0, BLOCK_V1, BLOCK_U1, BLOCK_V5);
						}
					}
				}

				// Top-left outer-corner
				if(block->next[4]->id != BLOCK_EMPTY)
				{
					if(block->next[4]->id != block->id && block->next[4]->id != block->next[3]->id && block->next[4]->id != block->next[5]->id)
					{
						m_tmpQuads[offset++] = BlockQuad(block->next[4]->id, BLOCK_X3, BLOCK_Y3, BLOCK_X4, BLOCK_Y4, BLOCK_U0, BLOCK_V5, BLOCK_U1, BLOCK_V6);
					}
				}

				// Draw top edge
				if(block->next[5]->id != BLOCK_EMPTY)
				{
					if(block->next[5]->id != block->id)
					{
						if(block->next[5]->id == block->next[3]->id)
						{
							if(block->next[5]->id == block->next[7]->id)
							{
								m_tmpQuads[offset++] = BlockQuad(block->next[5]->id, BLOCK_X1, BLOCK_Y3, BLOCK_X3, BLOCK_Y4, BLOCK_U2, BLOCK_V5, BLOCK_U4, BLOCK_V6);
								m_tmpQuads[offset++] = BlockQuad(block->next[5]->id, BLOCK_X3, BLOCK_Y3, BLOCK_X4, BLOCK_Y4, BLOCK_U1, BLOCK_V6, BLOCK_U2, BLOCK_V7);
								m_tmpQuads[offset++] = BlockQuad(block->next[5]->id, BLOCK_X0, BLOCK_Y3, BLOCK_X1, BLOCK_Y4, BLOCK_U0, BLOCK_V6, BLOCK_U1, BLOCK_V7);
							}
							else
							{
								m_tmpQuads[offset++] = BlockQuad(block->next[5]->id, BLOCK_X0, BLOCK_Y3, BLOCK_X3, BLOCK_Y4, BLOCK_U1, BLOCK_V5, BLOCK_U4, BLOCK_V6);
								m_tmpQuads[offset++] = BlockQuad(block->next[5]->id, BLOCK_X3, BLOCK_Y3, BLOCK_X4, BLOCK_Y4, BLOCK_U1, BLOCK_V6, BLOCK_U2, BLOCK_V7);
							}
						}
						else if(block->next[5]->id == block->next[7]->id)
						{
							m_tmpQuads[offset++] = BlockQuad(block->next[5]->id, BLOCK_X1, BLOCK_Y3, BLOCK_X4, BLOCK_Y4, BLOCK_U2, BLOCK_V5, BLOCK_U5, BLOCK_V6);
							m_tmpQuads[offset++] = BlockQuad(block->next[5]->id, BLOCK_X0, BLOCK_Y3, BLOCK_X1, BLOCK_Y4, BLOCK_U0, BLOCK_V6, BLOCK_U1, BLOCK_V7);
						}
						else
						{
							m_tmpQuads[offset++] = BlockQuad(block->next[5]->id, BLOCK_X0, BLOCK_Y3, BLOCK_X4, BLOCK_Y4, BLOCK_U1, BLOCK_V5, BLOCK_U5, BLOCK_V6);
						}
					}
				}
	
				// Top-right outer-corner
				if(block->next[6]->id != BLOCK_EMPTY)
				{
					if(block->next[6]->id != block->id && block->next[6]->id != block->next[7]->id && block->next[6]->id != block->next[5]->id)
					{
						m_tmpQuads[offset++] = BlockQuad(block->next[6]->id, BLOCK_X0, BLOCK_Y3, BLOCK_X1, BLOCK_Y4, BLOCK_U5, BLOCK_V5, BLOCK_U6, BLOCK_V6);
					}
				}

				// Draw right edge
				if(block->next[7]->id != BLOCK_EMPTY)
				{
					if(block->next[7]->id != block->id)
					{
						if(block->next[7]->id == block->next[1]->id)
						{
							if(block->next[7]->id == block->next[5]->id)
							{
								m_tmpQuads[offset++] = BlockQuad(block->next[7]->id, BLOCK_X0, BLOCK_Y1, BLOCK_X1, BLOCK_Y3, BLOCK_U5, BLOCK_V2, BLOCK_U6, BLOCK_V4);
							}
							else
							{
								m_tmpQuads[offset++] = BlockQuad(block->next[7]->id, BLOCK_X0, BLOCK_Y1, BLOCK_X1, BLOCK_Y4, BLOCK_U5, BLOCK_V2, BLOCK_U6, BLOCK_V5);
							}
						}
						else if(block->next[7]->id == block->next[5]->id)
						{
							m_tmpQuads[offset++] = BlockQuad(block->next[7]->id, BLOCK_X0, BLOCK_Y0, BLOCK_X1, BLOCK_Y3, BLOCK_U5, BLOCK_V2, BLOCK_U6, BLOCK_V5);
						}
						else
						{
							m_tmpQuads[offset++] = BlockQuad(block->next[7]->id, BLOCK_X0, BLOCK_Y0, BLOCK_X1, BLOCK_Y4, BLOCK_U5, BLOCK_V1, BLOCK_U6, BLOCK_V5);
						}
					}
				}

				if(offset > 0)
				{
					/*if(needsSorting)*/ sort(m_tmpQuads.begin(), m_tmpQuads.begin() + offset);

					XVertex *vertices = format.createVertices(4*offset); // TODO: Hmm... I don't like that this is so performance demanding as it is. I should concider a different way to represent vertices an vertex formats
					uint *indices = new uint[6*m_tmpQuads.size()];
					for(uint i = 0; i < offset; ++i)
					{
						BlockQuad &quad = m_tmpQuads[i];

						vertices[0 + i*4].set4f(VERTEX_POSITION, x + quad.x0, y + quad.y0);
						vertices[1 + i*4].set4f(VERTEX_POSITION, x + quad.x1, y + quad.y0);
						vertices[2 + i*4].set4f(VERTEX_POSITION, x + quad.x1, y + quad.y1);
						vertices[3 + i*4].set4f(VERTEX_POSITION, x + quad.x0, y + quad.y1);
	
						XTextureRegion region = BlockData::s_blockAtlas->get(quad.block, quad.u0, quad.v0, quad.u1, quad.v1);
						vertices[0 + i*4].set4f(VERTEX_TEX_COORD, region.uv0.x, region.uv1.y);
						vertices[1 + i*4].set4f(VERTEX_TEX_COORD, region.uv1.x, region.uv1.y);
						vertices[2 + i*4].set4f(VERTEX_TEX_COORD, region.uv1.x, region.uv0.y);
						vertices[3 + i*4].set4f(VERTEX_TEX_COORD, region.uv0.x, region.uv0.y);
		
						indices[0 + i*6] = QUAD_INDICES[0] + i*4;
						indices[1 + i*6] = QUAD_INDICES[1] + i*4;
						indices[2 + i*6] = QUAD_INDICES[2] + i*4;
						indices[3 + i*6] = QUAD_INDICES[3] + i*4;
						indices[4 + i*6] = QUAD_INDICES[4] + i*4;
						indices[5 + i*6] = QUAD_INDICES[5] + i*4;
					}
					m_vbo->addVertices(vertices, 4*offset, indices, 6*offset);

					delete[] vertices;
					delete[] indices;
				}


				//if(block > BLOCK_RESERVED) // no point in updating air/reserved blocks
				{
					//updateFixture(x, y, state);
				}
				//if(blockIsOpaque) break;
			}
		}
	}
	
	// Make the chunk buffer static
	m_vbo->setBufferType(XVertexBuffer::STATIC_BUFFER);
		
	//updateShadows();
}
	
void TerrainChunk::serialize(XFileWriter &ss)
{
	LOG("Saving chunk [%i, %i]...", m_x, m_y);
		
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
				Block *block = m_blocks[BLOCK_INDEX(x, y, i)];
				if(block->id <= BLOCK_OCCUPIED) block->id = BLOCK_EMPTY;
				ss << block->id << endl;
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
		
	LOG("Loading chunk [%i, %i]...", chunkX, chunkY);
		
	init(chunkX, chunkY);
		
	// Load blocks from stream
	for(int i = 0; i < TERRAIN_LAYER_COUNT; ++i)
	{
		for(int y = 0; y < CHUNK_BLOCKS; ++y)
		{
			for(int x = 0; x < CHUNK_BLOCKS; ++x)
			{
				int block;
				ss >> block;
				m_blocks[BLOCK_INDEX(x, y, i)] = new Block(BlockID(block));
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
	return m_state != CHUNK_DUMMY ? m_blocks[BLOCK_INDEX(x, y, layer)]->id : BLOCK_EMPTY;
}
	
bool TerrainChunk::isBlockAt(const int x, const int y, TerrainLayer layer) const
{
	return m_state != CHUNK_DUMMY && m_blocks[BLOCK_INDEX(x, y, layer)]->id != BLOCK_EMPTY;
}
	
bool TerrainChunk::isBlockOccupied(const int x, const int y, TerrainLayer layer) const
{
	return m_state != CHUNK_DUMMY && m_blocks[BLOCK_INDEX(x, y, layer)]->id >= BLOCK_OCCUPIED;
}
	
bool TerrainChunk::setBlockAt(const int x, const int y, const BlockID block, TerrainLayer layer)
{
	// Make sure we can add a block here
	if(m_state == CHUNK_INITIALIZED && m_blocks[BLOCK_INDEX(x, y, layer)]->id != block)
	{
		// Set the block value
		m_blocks[BLOCK_INDEX(x, y, layer)]->id = block;
		m_dirty = m_modified = true; // mark chunk as modified
		if(m_nextChunk[0] && y == 0)										m_nextChunk[0]->m_dirty = true;
		if(m_nextChunk[1] && x == CHUNK_BLOCKS-1 && y == 0)					m_nextChunk[1]->m_dirty = true;
		if(m_nextChunk[2] && x == CHUNK_BLOCKS-1)							m_nextChunk[2]->m_dirty = true;
		if(m_nextChunk[3] && x == CHUNK_BLOCKS-1 && y == CHUNK_BLOCKS-1)	m_nextChunk[3]->m_dirty = true;
		if(m_nextChunk[4] && y == CHUNK_BLOCKS-1)							m_nextChunk[4]->m_dirty = true;
		if(m_nextChunk[5] && x == 0 && y == CHUNK_BLOCKS-1)					m_nextChunk[5]->m_dirty = true;
		if(m_nextChunk[6] && x == 0)										m_nextChunk[6]->m_dirty = true;
		if(m_nextChunk[7] && x == 0 && y == 0)								m_nextChunk[7]->m_dirty = true;
		return true; // return true as something was changed
	}
	return false; // nothing changed
}

// SHADOWS
float TerrainChunk::getOpacity(const int x, const int y)
{
	float opacity = 0.0f;
	for(int i = TERRAIN_LAYER_COUNT-1; i >= 0; --i)
	{
		opacity += 0.0f;//BlockData::get(m_blocks[BLOCK_INDEX(x, y, i)]).getOpacity();
	}
	return opacity;
}

b2Vec2 tob2Vec(const Vector2 &vec)
{
	return b2Vec2(vec.x, vec.y);
}
	
// PHYSICS
void TerrainChunk::createFixture(const int x, const int y)
{
	//b2Fixture @fixture = @body.createFixture(Rect(x * BLOCK_PX - BLOCK_PX * 0.5f, y * BLOCK_PX - BLOCK_PX * 0.5f, BLOCK_PX*2, BLOCK_PX*2), 0.0f);
	Rect rect(x * BLOCK_PX, y * BLOCK_PX - 3, BLOCK_PX, BLOCK_PX + 3);

	b2PolygonShape shape;
	shape.SetAsBox(rect.getHeight()*0.5f, rect.getHeight()*0.5f, tob2Vec(rect.getCenter()), 0.0f);

	b2FixtureDef def;
	def.shape = &shape;
	def.density = 0.0f;

	b2Fixture *fixture = m_body->CreateFixture(&def);
	BlockData::get(getBlockAt(x, y, TERRAIN_LAYER_SCENE)).setupFixture(fixture);
	m_fixtures[FIXTURE_INDEX(x, y)] = fixture;
}
	
void TerrainChunk::removeFixture(const int x, const int y)
{
	/*body.removeFixture(@fixtures[x, y]);
	@fixtures[x, y] = null;*/
}
	
bool TerrainChunk::isFixtureAt(const int x, const int y)
{
	return /*state != CHUNK_DUMMY ? @fixtures[x, y] != null :*/ false;
}
	
void TerrainChunk::updateFixture(const int x, const int y, const uint state)
{
	// Find out if this block should contain a fixture
	bool shouldContainFixture = isBlockOccupied(x, y, TERRAIN_LAYER_SCENE) && (state & NESW) != NESW;
		
	// Create or remove fixture
	if(shouldContainFixture && !isFixtureAt(x, y))
	{
		createFixture(x, y);
	}
	else if(!shouldContainFixture && isFixtureAt(x, y))
	{
		removeFixture(x, y);
	}
}
	
void TerrainChunk::updateShadows()
{
	for(int y = -m_shadowRadius; y <= CHUNK_BLOCKS + m_shadowRadius; ++y)
	{
		for(int x = -m_shadowRadius; x <= CHUNK_BLOCKS + m_shadowRadius; ++x)
		{
			int blockX = m_x*CHUNK_BLOCKS + x, blockY = m_y*CHUNK_BLOCKS + y;
			/*float opacity = Terrain.getChunk(XMath::floor(blockX / CHUNK_BLOCKSF), XMath::floor(blockY / CHUNK_BLOCKSF)).getOpacity(XMath::mod(blockX, CHUNK_BLOCKS), XMath::mod(blockY, CHUNK_BLOCKS));
			array<Vector4> pixel = { Vector4(0.0f, 0.0f, 0.0f, opacity) };
			shadowMap.updateSection(x + shadowRadius, CHUNK_BLOCKS - y - 1 + shadowRadius, Pixmap(1, 1, pixel));*/
		}
	}
		
	XBatch batch;
		
	// Blur horizontally
	m_shadowPass1->clear();
	//blurHShader.setSampler2D("u_texture", @shadowMap);
	//blurHShader.setUniform1i("u_width", CHUNK_BLOCKS + shadowRadius*2);
	//blurHShader.setUniform1i("u_radius", shadowRadius-1);
	//batch.setShader(@blurHShader);
	XShape(Rect(0, 0, CHUNK_BLOCKS + m_shadowRadius*2, CHUNK_BLOCKS + m_shadowRadius*2)).draw(&batch);
	batch.renderToTexture(m_shadowPass1);
	batch.clear();
		
	// Blur vertically
	m_shadowPass2->clear();
	//blurVShader.setSampler2D("u_texture", @shadowPass1);
	//blurVShader.setSampler2D("u_filter", @shadowMap);
	//blurVShader.setUniform1i("u_height", CHUNK_BLOCKS + shadowRadius*2);
	//blurVShader.setUniform1i("u_radius", shadowRadius-1);
	//batch.setShader(@blurVShader);
	XShape(Rect(0, 0, CHUNK_BLOCKS + m_shadowRadius*2, CHUNK_BLOCKS + m_shadowRadius*2)).draw(&batch);
	batch.renderToTexture(m_shadowPass2);
	batch.clear();
}
	
// DRAWING
void TerrainChunk::draw(XBatch *batch)
{
	if(m_state == CHUNK_INITIALIZED)
	{
		if(m_dirty)
		{
			if(!m_nextChunksGenerated)
			{
				m_nextChunksGenerated = true;
				for(uint i = 0; i < 8; ++i)
				{
					if(m_nextChunk[i] == nullptr)
					{
						m_nextChunksGenerated = false;
						break;
					}
				}
			}
			
			if(m_nextChunksGenerated)
			{
				generateVBO();
				m_dirty = false;
			}
			else
			{
				return;
			}
		}
		
		// Draw blocks
		batch->setTexture(BlockData::s_blockAtlas->getTexture());
		batch->setPrimitive(XBatch::PRIMITIVE_TRIANGLES);
		batch->addVertexBuffer(m_vbo);
		
		// Draw shadows
		/*float f = m_shadowRadius/(CHUNK_BLOCKSF + m_shadowRadius*2);
		XSprite shadows(XTextureRegion(shared_ptr<XTexture>(m_shadowPass2), f, f, 1.0-f, 1.0-f));
		shadows.setPosition(CHUNK_PXF*m_x, CHUNK_PXF*m_y);
		shadows.setSize(CHUNK_PXF, CHUNK_PXF);*/
		//shadows.draw(Shadows);
	}
}