#include "blockdata.h"
#include "game/world.h"
#include "terrain/terrain.h"

vector<BlockData> BlockData::s_blockData(BLOCK_COUNT);
XTextureAtlas *BlockData::s_blockAtlas = nullptr;

void BlockData::init()
{
	shared_ptr<XTexture> temp = xd::ResourceManager::get<XTexture>(":/sprites/tiles/tile_template.png");
	s_blockData[BLOCK_EMPTY] = BlockData(BLOCK_EMPTY, xd::ResourceManager::get<XTexture>(":/sprites/tiles/empty_tile.png"), 0/*ITEM_GRASS_BLOCK*/, 1.0f);
	s_blockData[BLOCK_SCENE_GRASS] = BlockData(BLOCK_SCENE_GRASS, xd::ResourceManager::get<XTexture>(":/sprites/tiles/grass_tile.png"), 0/*ITEM_GRASS_BLOCK*/, 1.0f);

	vector<shared_ptr<XTexture>> textures;
	for(uint i = 0; i < BLOCK_COUNT; ++i)
	{
		textures.push_back(s_blockData[i].m_texture ? s_blockData[i].m_texture : temp);
	}

	s_blockAtlas = new XTextureAtlas(textures);
}

BlockData::BlockData() :
	m_id(BLOCK_NULL),
	m_texture(0),
	m_itemID(0),
	m_opacity(0.0f)
{
}

BlockData::BlockData(BlockID id, const shared_ptr<XTexture> &texture, const /*ItemID*/ uint i, const float opacity) :
	m_id(id),
	m_texture(texture),
	m_itemID(i),
	m_opacity(opacity)
{
}

uint BLOCK_INDICES[54] = {
	0, 3, 2, 0, 2, 1,       // q1
	4, 7, 6, 4, 6, 5,       // q2
	8, 11, 10, 8, 10, 9,    // q3
	12, 15, 14, 12, 14, 13, // q4
	16, 19, 18, 16, 18, 17, // q5
	20, 23, 22, 20, 22, 21, // q6
	24, 27, 26, 24, 26, 25, // q7
	28, 31, 30, 28, 30, 29, // q8
	32, 35, 34, 32, 34, 33  // q9
};

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

float BLOCK_VERTICES[9][4] =
{
	{ BLOCK_X0, BLOCK_Y0, BLOCK_X1, BLOCK_Y1 },
	{ BLOCK_X0, BLOCK_Y0, BLOCK_X3, BLOCK_Y1 },
	{ BLOCK_X2, BLOCK_Y0, BLOCK_X3, BLOCK_Y1 },
	{ BLOCK_X0, BLOCK_Y0, BLOCK_X1, BLOCK_Y3 },
	{ BLOCK_X2, BLOCK_Y0, BLOCK_X3, BLOCK_Y3 },
	{ BLOCK_X0, BLOCK_Y2, BLOCK_X1, BLOCK_Y3 },
	{ BLOCK_X0, BLOCK_Y2, BLOCK_X3, BLOCK_Y3 },
	{ BLOCK_X2, BLOCK_Y2, BLOCK_X3, BLOCK_Y3 },
	{ BLOCK_X0, BLOCK_Y0, BLOCK_X3, BLOCK_Y3 }
};

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

struct BlockQuad
{
	BlockQuad(BlockID block, const float x0, const float y0, const float x1, const float y1, const float u0, const float v0, const float u1, const float v1) :
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
	
	BlockID block;
	float x0, y0, x1, y1, u0, v0, u1, v1;
};

void BlockData::getVertices(const int x, const int y, const BlockID *blocks, shared_ptr<XVertexBuffer> buffer) const
{
	XVertexFormat &format = World::getTerrain()->getVertexFormat();

	vector<BlockQuad> quads;
	if(m_id != BLOCK_EMPTY)
	{
		quads.push_back(BlockQuad(m_id, BLOCK_X0, BLOCK_Y0, BLOCK_X4, BLOCK_Y4, BLOCK_U1, BLOCK_V1, BLOCK_U5, BLOCK_V5));
	}
	
	// Bottom-right outer-corner
	if(blocks[1] != m_id && blocks[1] != blocks[2] && blocks[1] != blocks[8])
	{
		quads.push_back(BlockQuad(blocks[1], BLOCK_X0, BLOCK_Y0, BLOCK_X1, BLOCK_Y1, BLOCK_U5, BLOCK_V0, BLOCK_U6, BLOCK_V1));
	}

	// Draw bottom edge
	if(blocks[2] != blocks[0])
	{
		if(blocks[2] == blocks[4])
		{
			if(blocks[2] == blocks[8])
			{
				quads.push_back(BlockQuad(blocks[2], BLOCK_X1, BLOCK_Y0, BLOCK_X3, BLOCK_Y1, BLOCK_U2, BLOCK_V0, BLOCK_U4, BLOCK_V1));
				quads.push_back(BlockQuad(blocks[2], BLOCK_X3, BLOCK_Y0, BLOCK_X4, BLOCK_Y1, BLOCK_U1, BLOCK_V7, BLOCK_U2, BLOCK_V8));
				quads.push_back(BlockQuad(blocks[2], BLOCK_X0, BLOCK_Y0, BLOCK_X1, BLOCK_Y1, BLOCK_U0, BLOCK_V7, BLOCK_U1, BLOCK_V8));
			}
			else
			{
				quads.push_back(BlockQuad(blocks[2], BLOCK_X0, BLOCK_Y0, BLOCK_X3, BLOCK_Y1, BLOCK_U1, BLOCK_V0, BLOCK_U4, BLOCK_V1));
				quads.push_back(BlockQuad(blocks[2], BLOCK_X3, BLOCK_Y0, BLOCK_X4, BLOCK_Y1, BLOCK_U1, BLOCK_V7, BLOCK_U2, BLOCK_V8));
			}
		}
		else if(blocks[2] == blocks[8])
		{
			quads.push_back(BlockQuad(blocks[2], BLOCK_X1, BLOCK_Y0, BLOCK_X4, BLOCK_Y1, BLOCK_U2, BLOCK_V0, BLOCK_U5, BLOCK_V1));
			quads.push_back(BlockQuad(blocks[2], BLOCK_X0, BLOCK_Y0, BLOCK_X1, BLOCK_Y1, BLOCK_U0, BLOCK_V7, BLOCK_U1, BLOCK_V8));
		}
		else
		{
			quads.push_back(BlockQuad(blocks[2], BLOCK_X0, BLOCK_Y0, BLOCK_X4, BLOCK_Y1, BLOCK_U1, BLOCK_V0, BLOCK_U5, BLOCK_V1));
		}
	}

	// Bottom-left outer-corner
	if(blocks[3] != m_id && blocks[3] != blocks[2] && blocks[3] != blocks[4])
	{
		quads.push_back(BlockQuad(blocks[3], BLOCK_X3, BLOCK_Y0, BLOCK_X4, BLOCK_Y1, BLOCK_U0, BLOCK_V0, BLOCK_U1, BLOCK_V1));
	}

	// Draw left edge
	if(blocks[4] != blocks[0])
	{
		if(blocks[4] == blocks[2])
		{
			if(blocks[4] == blocks[6])
			{
				quads.push_back(BlockQuad(blocks[4], BLOCK_X3, BLOCK_Y1, BLOCK_X4, BLOCK_Y3, BLOCK_U0, BLOCK_V2, BLOCK_U1, BLOCK_V4));
			}
			else
			{
				quads.push_back(BlockQuad(blocks[4], BLOCK_X3, BLOCK_Y1, BLOCK_X4, BLOCK_Y4, BLOCK_U0, BLOCK_V2, BLOCK_U1, BLOCK_V5));
			}
		}
		else if(blocks[4] == blocks[6])
		{
			quads.push_back(BlockQuad(blocks[4], BLOCK_X3, BLOCK_Y0, BLOCK_X4, BLOCK_Y3, BLOCK_U0, BLOCK_V2, BLOCK_U1, BLOCK_V5));
		}
		else
		{
			quads.push_back(BlockQuad(blocks[4], BLOCK_X3, BLOCK_Y0, BLOCK_X4, BLOCK_Y4, BLOCK_U0, BLOCK_V1, BLOCK_U1, BLOCK_V5));
		}
	}

	// Top-left outer-corner
	if(blocks[5] != m_id && blocks[5] != blocks[4] && blocks[5] != blocks[6])
	{
		quads.push_back(BlockQuad(blocks[5], BLOCK_X3, BLOCK_Y3, BLOCK_X4, BLOCK_Y4, BLOCK_U0, BLOCK_V5, BLOCK_U1, BLOCK_V6));
	}

	// Draw top edge
	if(blocks[6] != blocks[0])
	{
		if(blocks[6] == blocks[4])
		{
			if(blocks[6] == blocks[8])
			{
				quads.push_back(BlockQuad(blocks[6], BLOCK_X1, BLOCK_Y3, BLOCK_X3, BLOCK_Y4, BLOCK_U2, BLOCK_V5, BLOCK_U4, BLOCK_V6));
				quads.push_back(BlockQuad(blocks[6], BLOCK_X3, BLOCK_Y3, BLOCK_X4, BLOCK_Y4, BLOCK_U1, BLOCK_V6, BLOCK_U2, BLOCK_V7));
				quads.push_back(BlockQuad(blocks[6], BLOCK_X0, BLOCK_Y3, BLOCK_X1, BLOCK_Y4, BLOCK_U0, BLOCK_V6, BLOCK_U1, BLOCK_V7));
			}
			else
			{
				quads.push_back(BlockQuad(blocks[6], BLOCK_X0, BLOCK_Y3, BLOCK_X3, BLOCK_Y4, BLOCK_U1, BLOCK_V5, BLOCK_U4, BLOCK_V6));
				quads.push_back(BlockQuad(blocks[4], BLOCK_X3, BLOCK_Y3, BLOCK_X4, BLOCK_Y4, BLOCK_U1, BLOCK_V6, BLOCK_U2, BLOCK_V7));
			}
		}
		else if(blocks[6] == blocks[8])
		{
			quads.push_back(BlockQuad(blocks[6], BLOCK_X1, BLOCK_Y3, BLOCK_X4, BLOCK_Y4, BLOCK_U2, BLOCK_V5, BLOCK_U5, BLOCK_V6));
			quads.push_back(BlockQuad(blocks[6], BLOCK_X0, BLOCK_Y3, BLOCK_X1, BLOCK_Y4, BLOCK_U0, BLOCK_V6, BLOCK_U1, BLOCK_V7));
		}
		else
		{
			quads.push_back(BlockQuad(blocks[6], BLOCK_X0, BLOCK_Y3, BLOCK_X4, BLOCK_Y4, BLOCK_U1, BLOCK_V5, BLOCK_U5, BLOCK_V6));
		}
	}
	
	// Top-right outer-corner
	if(blocks[7] != m_id && blocks[7] != blocks[8] && blocks[7] != blocks[6])
	{
		quads.push_back(BlockQuad(blocks[7], BLOCK_X0, BLOCK_Y3, BLOCK_X1, BLOCK_Y4, BLOCK_U5, BLOCK_V5, BLOCK_U6, BLOCK_V6));
	}

	// Draw right edge
	if(blocks[8] != blocks[0])
	{
		if(blocks[8] == blocks[2])
		{
			if(blocks[8] == blocks[6])
			{
				quads.push_back(BlockQuad(blocks[8], BLOCK_X0, BLOCK_Y1, BLOCK_X1, BLOCK_Y3, BLOCK_U5, BLOCK_V2, BLOCK_U6, BLOCK_V4));
			}
			else
			{
				quads.push_back(BlockQuad(blocks[8], BLOCK_X0, BLOCK_Y1, BLOCK_X1, BLOCK_Y4, BLOCK_U5, BLOCK_V2, BLOCK_U6, BLOCK_V5));
			}
		}
		else if(blocks[8] == blocks[6])
		{
			quads.push_back(BlockQuad(blocks[8], BLOCK_X0, BLOCK_Y0, BLOCK_X1, BLOCK_Y3, BLOCK_U5, BLOCK_V2, BLOCK_U6, BLOCK_V5));
		}
		else
		{
			quads.push_back(BlockQuad(blocks[8], BLOCK_X0, BLOCK_Y0, BLOCK_X1, BLOCK_Y4, BLOCK_U5, BLOCK_V1, BLOCK_U6, BLOCK_V5));
		}
	}

	if(quads.size() > 0)
	{
		XVertex *vertices = format.createVertices(4*quads.size());
		uint *indices = new uint[6*quads.size()];
		for(uint i = 0; i < quads.size(); ++i)
		{
			BlockQuad &quad = quads[i];

			vertices[0 + i*4].set4f(VERTEX_POSITION, x + quad.x0, y + quad.y0);
			vertices[1 + i*4].set4f(VERTEX_POSITION, x + quad.x1, y + quad.y0);
			vertices[2 + i*4].set4f(VERTEX_POSITION, x + quad.x1, y + quad.y1);
			vertices[3 + i*4].set4f(VERTEX_POSITION, x + quad.x0, y + quad.y1);
	
			XTextureRegion region = s_blockAtlas->get(quad.block, quad.u0, quad.v0, quad.u1, quad.v1);
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
		buffer->addVertices(vertices, 4*quads.size(), indices, 6*quads.size());

		delete[] vertices;
		delete[] indices;
	}
}

BlockData &BlockData::get(const BlockID tile)
{
	return s_blockData[tile];
}