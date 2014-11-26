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
#define BLOCK_X0 0.0f
#define BLOCK_Y0 0.0f
#define BLOCK_X1 BORDER_PXF/FULL_BLOCK_PXF
#define BLOCK_Y1 BORDER_PXF/FULL_BLOCK_PXF
#define BLOCK_X2 (FULL_BLOCK_PXF-BORDER_PXF)/FULL_BLOCK_PXF
#define BLOCK_Y2 (FULL_BLOCK_PXF-BORDER_PXF)/FULL_BLOCK_PXF
#define BLOCK_X3 1.0f
#define BLOCK_Y3 1.0f

float BLOCK_VERTICES[9][4] =
{
	{ BLOCK_X0, BLOCK_Y0, BLOCK_X1, BLOCK_Y1 },
	{ BLOCK_X1, BLOCK_Y0, BLOCK_X2, BLOCK_Y1 },
	{ BLOCK_X2, BLOCK_Y0, BLOCK_X3, BLOCK_Y1 },
	{ BLOCK_X0, BLOCK_Y1, BLOCK_X1, BLOCK_Y2 },
	{ BLOCK_X2, BLOCK_Y1, BLOCK_X3, BLOCK_Y2 },
	{ BLOCK_X0, BLOCK_Y2, BLOCK_X1, BLOCK_Y3 },
	{ BLOCK_X1, BLOCK_Y2, BLOCK_X2, BLOCK_Y3 },
	{ BLOCK_X2, BLOCK_Y2, BLOCK_X3, BLOCK_Y3 },
	{ BLOCK_X0, BLOCK_Y0, BLOCK_X3, BLOCK_Y3 }
};

#define BLOCK_U0 0.0f
#define BLOCK_V0 0.0f
#define BLOCK_U1 BORDER_PXF/FULL_BLOCK_PXF
#define BLOCK_V1 BORDER_PXF/FULL_BLOCK_PXF
#define BLOCK_U2 (FULL_BLOCK_PXF-BORDER_PXF)/FULL_BLOCK_PXF
#define BLOCK_V2 (FULL_BLOCK_PXF-BORDER_PXF)/FULL_BLOCK_PXF
#define BLOCK_U3 1.0f
#define BLOCK_V3 1.0f

float BLOCK_TEXTURE_COORDS[9][4] =
{
	{ BLOCK_U2, BLOCK_V0, BLOCK_U3, BLOCK_V1 }, // 0
	{ BLOCK_U1, BLOCK_V0, BLOCK_U2, BLOCK_V1 }, // 1
	{ BLOCK_U0, BLOCK_V0, BLOCK_U1, BLOCK_V1 }, // 2
	{ BLOCK_U2, BLOCK_V1, BLOCK_U3, BLOCK_V2 }, // 3
	{ BLOCK_U0, BLOCK_V1, BLOCK_U1, BLOCK_V2 }, // 4
	{ BLOCK_U2, BLOCK_V2, BLOCK_U3, BLOCK_V3 }, // 5
	{ BLOCK_U1, BLOCK_V2, BLOCK_U2, BLOCK_V3 }, // 6
	{ BLOCK_U0, BLOCK_V2, BLOCK_U1, BLOCK_V3 }, // 7
	{ BLOCK_U1, BLOCK_V1, BLOCK_U2, BLOCK_V2 }  // 8
};

uint BLOCK_DIR[8] =
{
	NORTH_WEST,
	NORTH,
	NORTH_EAST,
	WEST,
	EAST,
	SOUTH_WEST,
	SOUTH,
	SOUTH_EAST
};

void BlockData::getVertices(const int x, const int y, const BlockID *blocks, shared_ptr<XVertexBuffer> buffer) const
{
	/*array<Vertex> vertices = Terrain.getVertexFormat().createVertices(4*9);
	TextureAtlas @atlas = @Tiles.getAtlas();
	for(int i = 0; i < 9; ++i)
	{
		if(i >= 0 && i <= 2 && state & NORTH != 0) continue;
		else if(i >= 2 && i <= 4 && state & EAST != 0) continue;
		else if(i >= 4 && i <= 6 && state & SOUTH != 0) continue;
		else if((i >= 6 && i <= 7 || i == 0) && state & WEST != 0) continue;
			
		vertices[i*4+0].set4f(VERTEX_POSITION, x * TILE_PX + (TILE_TEXTURE_COORDS[0, i] * FULL_TILE_PXF - BORDER_PXF), y * TILE_PX + (TILE_TEXTURE_COORDS[1, i] * FULL_TILE_PXF - BORDER_PXF));
		vertices[i*4+1].set4f(VERTEX_POSITION, x * TILE_PX + (TILE_TEXTURE_COORDS[2, i] * FULL_TILE_PXF - BORDER_PXF), y * TILE_PX + (TILE_TEXTURE_COORDS[1, i] * FULL_TILE_PXF - BORDER_PXF));
		vertices[i*4+2].set4f(VERTEX_POSITION, x * TILE_PX + (TILE_TEXTURE_COORDS[2, i] * FULL_TILE_PXF - BORDER_PXF), y * TILE_PX + (TILE_TEXTURE_COORDS[3, i] * FULL_TILE_PXF - BORDER_PXF));
		vertices[i*4+3].set4f(VERTEX_POSITION, x * TILE_PX + (TILE_TEXTURE_COORDS[0, i] * FULL_TILE_PXF - BORDER_PXF), y * TILE_PX + (TILE_TEXTURE_COORDS[3, i] * FULL_TILE_PXF - BORDER_PXF));
			
		TextureRegion region = atlas.get(id, TILE_TEXTURE_COORDS[0, i], 1-TILE_TEXTURE_COORDS[3, i], TILE_TEXTURE_COORDS[2, i], 1-TILE_TEXTURE_COORDS[1, i]);
		vertices[i*4+0].set4f(VERTEX_TEX_COORD, region.uv0.x, region.uv1.y);
		vertices[i*4+1].set4f(VERTEX_TEX_COORD, region.uv1.x, region.uv1.y);
		vertices[i*4+2].set4f(VERTEX_TEX_COORD, region.uv1.x, region.uv0.y);
		vertices[i*4+3].set4f(VERTEX_TEX_COORD, region.uv0.x, region.uv0.y);
	}
	return vertices;*/

	XVertexFormat &format = World::getTerrain()->getVertexFormat();

	if(m_id != BLOCK_EMPTY)
	{
		XVertex *vertices = format.createVertices(4);
		uint *indices = new uint[6];

		vertices[0].set4f(VERTEX_POSITION, x + BLOCK_VERTICES[8][0], y + BLOCK_VERTICES[8][1]);
		vertices[1].set4f(VERTEX_POSITION, x + BLOCK_VERTICES[8][2], y + BLOCK_VERTICES[8][1]);
		vertices[2].set4f(VERTEX_POSITION, x + BLOCK_VERTICES[8][2], y + BLOCK_VERTICES[8][3]);
		vertices[3].set4f(VERTEX_POSITION, x + BLOCK_VERTICES[8][0], y + BLOCK_VERTICES[8][3]);
	
		XTextureRegion region = s_blockAtlas->get(m_id, BLOCK_TEXTURE_COORDS[8][0], BLOCK_TEXTURE_COORDS[8][1], BLOCK_TEXTURE_COORDS[8][2], BLOCK_TEXTURE_COORDS[8][3]);
		vertices[0].set4f(VERTEX_TEX_COORD, region.uv0.x, region.uv1.y);
		vertices[1].set4f(VERTEX_TEX_COORD, region.uv1.x, region.uv1.y);
		vertices[2].set4f(VERTEX_TEX_COORD, region.uv1.x, region.uv0.y);
		vertices[3].set4f(VERTEX_TEX_COORD, region.uv0.x, region.uv0.y);
		
		memcpy(indices, QUAD_INDICES, 6*sizeof(uint));

		buffer->addVertices(vertices, 4, indices, 6);

		delete[] vertices;
		delete[] indices;
	}

	for(uint i = 0; i < 8; ++i)
	{
		if(blocks[i] != BLOCK_EMPTY && blocks[i] != m_id)
		{
			XVertex *vertices = format.createVertices(4);
			uint *indices = new uint[6];

			vertices[0].set4f(VERTEX_POSITION, x + BLOCK_VERTICES[i][0], y + BLOCK_VERTICES[i][1]);
			vertices[1].set4f(VERTEX_POSITION, x + BLOCK_VERTICES[i][2], y + BLOCK_VERTICES[i][1]);
			vertices[2].set4f(VERTEX_POSITION, x + BLOCK_VERTICES[i][2], y + BLOCK_VERTICES[i][3]);
			vertices[3].set4f(VERTEX_POSITION, x + BLOCK_VERTICES[i][0], y + BLOCK_VERTICES[i][3]);
	
			XTextureRegion region = s_blockAtlas->get(blocks[i], BLOCK_TEXTURE_COORDS[i][0], BLOCK_TEXTURE_COORDS[i][1], BLOCK_TEXTURE_COORDS[i][2], BLOCK_TEXTURE_COORDS[i][3]);
			vertices[0].set4f(VERTEX_TEX_COORD, region.uv0.x, region.uv1.y);
			vertices[1].set4f(VERTEX_TEX_COORD, region.uv1.x, region.uv1.y);
			vertices[2].set4f(VERTEX_TEX_COORD, region.uv1.x, region.uv0.y);
			vertices[3].set4f(VERTEX_TEX_COORD, region.uv0.x, region.uv0.y);
		
			memcpy(indices, QUAD_INDICES, 6*sizeof(uint));

			buffer->addVertices(vertices, 4, indices, 6);

			delete[] vertices;
			delete[] indices;
		}
	}
}

BlockData &BlockData::get(const BlockID tile)
{
	return s_blockData[tile];
}