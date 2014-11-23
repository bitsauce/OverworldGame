#include "tiledata.h"

/*array<uint> TILE_INDICES = {
	0, 3, 2, 0, 2, 1,       // q1
	4, 7, 6, 4, 6, 5,       // q2
	8, 11, 10, 8, 10, 9,    // q3
	12, 15, 14, 12, 14, 13, // q4
	16, 19, 18, 16, 18, 17, // q5
	20, 23, 22, 20, 22, 21, // q6
	24, 27, 26, 24, 26, 25, // q7
	28, 31, 30, 28, 30, 29, // q8
	32, 35, 34, 32, 34, 33  // q9
};*/

vector<XVertex> Tile::getVertices(const int x, const int y, const uint state)
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
	return vector<XVertex>();
}

vector<uint> Tile::getIndices()
{
	return vector<uint>();
	//return TILE_INDICES;
}

Tile &TileData::get(const BlockID tile)
{
	return Tile();
}