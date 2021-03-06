in vec2 v_TexCoord;
out uvec4 out_BlockData;
out uvec4 out_QuadData;

uniform sampler2D u_BlockGrid;
uniform uint u_Layer;

#define QUAD_0 12U
#define QUAD_1 13U
#define QUAD_2 14U
#define QUAD_3 15U
#define QUAD_4 8U
#define QUAD_5 9U
#define QUAD_6 10U
#define QUAD_7 11U
#define QUAD_8 4U
#define QUAD_9 5U
#define QUAD_10 6U
#define QUAD_11 7U
#define QUAD_12 0U
#define QUAD_13 1U
#define QUAD_14 2U
#define QUAD_15 3U
#define QUAD_16 18U
#define QUAD_17 19U
#define QUAD_18 16U
#define QUAD_19 17U

uvec3 quad[4];
uvec3 tmp;

void main()
{
	// Get block IDs/depths
	ivec2 tileMapSize = textureSize(u_BlockGrid, 0);
	uint centerBlockID		= uint(texture(u_BlockGrid, v_TexCoord)[u_Layer] * 255.5);
	uint leftBlockID		= uint(texture(u_BlockGrid, v_TexCoord + vec2(-1.0 / tileMapSize.x, 0.0))[u_Layer] * 255.5);
	uint rightBlockID		= uint(texture(u_BlockGrid, v_TexCoord + vec2( 1.0 / tileMapSize.x, 0.0))[u_Layer] * 255.5);
	uint topBlockID			= uint(texture(u_BlockGrid, v_TexCoord + vec2(0.0,  1.0 / tileMapSize.y))[u_Layer] * 255.5);
	uint bottomBlockID		= uint(texture(u_BlockGrid, v_TexCoord + vec2(0.0, -1.0 / tileMapSize.y))[u_Layer] * 255.5);
	uint topLeftBlockID		= uint(texture(u_BlockGrid, v_TexCoord + vec2(-1.0 / tileMapSize.x,  1.0 / tileMapSize.y))[u_Layer] * 255.5);
	uint topRightBlockID	= uint(texture(u_BlockGrid, v_TexCoord + vec2( 1.0 / tileMapSize.x,  1.0 / tileMapSize.y))[u_Layer] * 255.5);
	uint bottomLeftBlockID	= uint(texture(u_BlockGrid, v_TexCoord + vec2(-1.0 / tileMapSize.x, -1.0 / tileMapSize.y))[u_Layer] * 255.5);
	uint bottomRightBlockID	= uint(texture(u_BlockGrid, v_TexCoord + vec2( 1.0 / tileMapSize.x, -1.0 / tileMapSize.y))[u_Layer] * 255.5);

	// Compare diagonal blocks to check for inner corners
	bvec4 innerCorner = equal(uvec4(bottomBlockID, bottomBlockID, topBlockID, topBlockID), uvec4(leftBlockID, rightBlockID, leftBlockID, rightBlockID));
	
	// Sort bottom-left quads
	quad[0] = uvec3(0, centerBlockID, QUAD_9);	// NOTE TO SELF: The 1st parameter is unused, but can be usefull for when
												// I want a separate depth variable for my blocks (instead of using the id)
	quad[1] = uvec3(1, bottomBlockID, innerCorner[0] == true ? QUAD_18 : QUAD_1);
	quad[2] = uvec3(2, leftBlockID, QUAD_11);
	quad[3] = uvec3(3, bottomLeftBlockID, QUAD_3);
	for(int i = 0; i < 4; ++i)
	{
		tmp = quad[i];
		int j = i;
		while(j > 0 && quad[j - 1][1] > tmp[1])
		{
			quad[j] = quad[j - 1];
			--j;
		}
		quad[j] = tmp;
	}
	out_BlockData[0] = quad[0][1] << 24 | quad[1][1] << 16 | quad[2][1] << 8 | quad[3][1];
	out_QuadData [0] = quad[0][2] << 24 | quad[1][2] << 16 | quad[2][2] << 8 | quad[3][2];

	// Sort bottom-right quads
	quad[0] = uvec3(0, centerBlockID, QUAD_10);
	quad[1] = uvec3(1, bottomBlockID, innerCorner[1] == true ? QUAD_19 : QUAD_2);
	quad[2] = uvec3(2, rightBlockID, QUAD_8);
	quad[3] = uvec3(3, bottomRightBlockID, QUAD_0);
	for(int i = 0; i < 4; ++i)
	{
		tmp = quad[i];
		int j = i;
		while(j > 0 && quad[j - 1][1] > tmp[1])
		{
			quad[j] = quad[j - 1];
			--j;
		}
		quad[j] = tmp;
	}
	out_BlockData[1] = quad[0][1] << 24 | quad[1][1] << 16 | quad[2][1] << 8 | quad[3][1];
	out_QuadData [1] = quad[0][2] << 24 | quad[1][2] << 16 | quad[2][2] << 8 | quad[3][2];

	// Sort top-left quads
	quad[0] = uvec3(0, centerBlockID, QUAD_5);
	quad[1] = uvec3(1, topBlockID, innerCorner[2] == true ? QUAD_16 : QUAD_13);
	quad[2] = uvec3(2, leftBlockID, QUAD_7);
	quad[3] = uvec3(3, topLeftBlockID, QUAD_15);
	for(int i = 0; i < 4; ++i)
	{
		tmp = quad[i];
		int j = i;
		while(j > 0 && quad[j - 1][1] > tmp[1])
		{
			quad[j] = quad[j - 1];
			--j;
		}
		quad[j] = tmp;
	}
	out_BlockData[2] = (quad[0][1] << 24) | (quad[1][1] << 16) | (quad[2][1] << 8) | quad[3][1];
	out_QuadData [2] = (quad[0][2] << 24) | (quad[1][2] << 16) | (quad[2][2] << 8) | quad[3][2];

	// Sort top-right quads
	quad[0] = uvec3(0, centerBlockID, QUAD_6);
	quad[1] = uvec3(1, topBlockID, innerCorner[3] == true ? QUAD_17 : QUAD_14);
	quad[2] = uvec3(2, rightBlockID, QUAD_4);
	quad[3] = uvec3(3, topRightBlockID, QUAD_12);
	for(int i = 0; i < 4; ++i)
	{
		tmp = quad[i];
		int j = i;
		while(j > 0 && quad[j - 1][1] > tmp[1])
		{
			quad[j] = quad[j - 1];
			--j;
		}
		quad[j] = tmp;
	}
	out_BlockData[3] = quad[0][1] << 24 | quad[1][1] << 16 | quad[2][1] << 8 | quad[3][1];
	out_QuadData [3] = quad[0][2] << 24 | quad[1][2] << 16 | quad[2][2] << 8 | quad[3][2];
}