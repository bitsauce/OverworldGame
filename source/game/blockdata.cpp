#include "blockdata.h"
#include "game/world.h"
#include "terrain/terrain.h"

vector<BlockData> BlockData::s_blockData(BLOCK_COUNT);
XTextureAtlas *BlockData::s_blockAtlas = nullptr;

void BlockData::init()
{
	shared_ptr<XTexture> temp = xd::ResourceManager::get<XTexture>(":/sprites/tiles/tile_template.png");
	s_blockData[BLOCK_EMPTY] = BlockData(BLOCK_EMPTY, xd::ResourceManager::get<XTexture>(":/sprites/tiles/empty_tile.png"), 0/*ITEM_GRASS_BLOCK*/, 1.0f);
	s_blockData[BLOCK_SCENE_GRASS] = BlockData(BLOCK_SCENE_GRASS, xd::ResourceManager::get<XTexture>(":/sprites/tiles/tile_template.png"), 0/*ITEM_GRASS_BLOCK*/, 1.0f);

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

#define BLOCK_UV0 (0.0f / 6.0f)
#define BLOCK_UV1 (1.0f / 6.0f)
#define BLOCK_UV2 (2.0f / 6.0f)
#define BLOCK_UV3 (3.0f / 6.0f)
#define BLOCK_UV4 (4.0f / 6.0f)
#define BLOCK_UV5 (5.0f / 6.0f)
#define BLOCK_UV6 (6.0f / 6.0f)

uint BLOCK_DIR[8] =
{
	NORTH_WEST, // 0
	NORTH,      // 1
	NORTH_EAST, // 2
	WEST,       // 3
	EAST,       // 4
	SOUTH_WEST, // 5
	SOUTH,      // 6
	SOUTH_EAST  // 7
};

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
		quads.push_back(BlockQuad(m_id, BLOCK_X0, BLOCK_Y0, BLOCK_X4, BLOCK_Y4, BLOCK_UV1, BLOCK_UV1, BLOCK_UV5, BLOCK_UV5));
	}

	// Draw left edge
	if(blocks[4] != blocks[0])
	{
		if(blocks[4] == blocks[2])
			if(blocks[4] == blocks[6])
				quads.push_back(BlockQuad(blocks[4], BLOCK_X3, BLOCK_Y1, BLOCK_X4, BLOCK_Y3, BLOCK_UV0, BLOCK_UV2, BLOCK_UV1, BLOCK_UV4));
			else
				quads.push_back(BlockQuad(blocks[4], BLOCK_X3, BLOCK_Y1, BLOCK_X4, BLOCK_Y4, BLOCK_UV0, BLOCK_UV2, BLOCK_UV1, BLOCK_UV5));
		else if(blocks[4] == blocks[6])
			quads.push_back(BlockQuad(blocks[4], BLOCK_X3, BLOCK_Y0, BLOCK_X4, BLOCK_Y3, BLOCK_UV0, BLOCK_UV2, BLOCK_UV1, BLOCK_UV5));
		else
			quads.push_back(BlockQuad(blocks[4], BLOCK_X3, BLOCK_Y0, BLOCK_X4, BLOCK_Y4, BLOCK_UV0, BLOCK_UV1, BLOCK_UV1, BLOCK_UV5));
	}

	// Top-left outer-corner
	if(blocks[5] != m_id && blocks[5] != blocks[4] && blocks[5] != blocks[6])
	{
		quads.push_back(BlockQuad(blocks[5], BLOCK_X3, BLOCK_Y3, BLOCK_X4, BLOCK_Y4, BLOCK_UV0, BLOCK_UV5, BLOCK_UV1, BLOCK_UV6));
	}

	// Draw top edge
	if(blocks[6] != blocks[0])
	{
		if(blocks[6] == blocks[4])
			if(blocks[6] == blocks[8])
				quads.push_back(BlockQuad(blocks[6], BLOCK_X1, BLOCK_Y3, BLOCK_X3, BLOCK_Y4, BLOCK_UV2, BLOCK_UV5, BLOCK_UV4, BLOCK_UV6));
			else
				quads.push_back(BlockQuad(blocks[6], BLOCK_X0, BLOCK_Y3, BLOCK_X3, BLOCK_Y4, BLOCK_UV1, BLOCK_UV5, BLOCK_UV4, BLOCK_UV6));
		else if(blocks[6] == blocks[8])
			quads.push_back(BlockQuad(blocks[6], BLOCK_X1, BLOCK_Y3, BLOCK_X4, BLOCK_Y4, BLOCK_UV2, BLOCK_UV5, BLOCK_UV5, BLOCK_UV6));
		else
			quads.push_back(BlockQuad(blocks[6], BLOCK_X0, BLOCK_Y3, BLOCK_X4, BLOCK_Y4, BLOCK_UV1, BLOCK_UV5, BLOCK_UV5, BLOCK_UV6));
	}
	
	// Top-right outer-corner
	if(blocks[7] != m_id && blocks[7] != blocks[8] && blocks[7] != blocks[6])
	{
		quads.push_back(BlockQuad(blocks[7], BLOCK_X0, BLOCK_Y3, BLOCK_X1, BLOCK_Y4, BLOCK_UV5, BLOCK_UV5, BLOCK_UV6, BLOCK_UV6));
	}

	// Draw right edge
	if(blocks[8] != blocks[0])
	{
		if(blocks[8] == blocks[2])
			if(blocks[8] == blocks[6])
				quads.push_back(BlockQuad(blocks[8], BLOCK_X0, BLOCK_Y1, BLOCK_X1, BLOCK_Y3, BLOCK_UV5, BLOCK_UV2, BLOCK_UV6, BLOCK_UV4));
			else
				quads.push_back(BlockQuad(blocks[8], BLOCK_X0, BLOCK_Y1, BLOCK_X1, BLOCK_Y4, BLOCK_UV5, BLOCK_UV2, BLOCK_UV6, BLOCK_UV5));
		else if(blocks[8] == blocks[6])
			quads.push_back(BlockQuad(blocks[8], BLOCK_X0, BLOCK_Y0, BLOCK_X1, BLOCK_Y3, BLOCK_UV5, BLOCK_UV2, BLOCK_UV6, BLOCK_UV5));
		else
			quads.push_back(BlockQuad(blocks[8], BLOCK_X0, BLOCK_Y0, BLOCK_X1, BLOCK_Y4, BLOCK_UV5, BLOCK_UV1, BLOCK_UV6, BLOCK_UV5));
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