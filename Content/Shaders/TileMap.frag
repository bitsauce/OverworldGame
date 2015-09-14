in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform usampler2D u_TileMap;
uniform sampler2D u_BlockAtlas;

#define BLOCK_PX 16.0
#define CHUNK_BLOCKS 32.0
#define CHUNK_PX (CHUNK_BLOCKS * BLOCK_PX)

#define QUAD_0 0
#define QUAD_1 1
#define QUAD_2 2
#define QUAD_3 3
#define QUAD_4 4
#define QUAD_5 5
#define QUAD_6 6
#define QUAD_7 7
#define QUAD_8 8
#define QUAD_9 9
#define QUAD_10 10
#define QUAD_11 11
#define QUAD_12 12
#define QUAD_13 13
#define QUAD_14 14
#define QUAD_15 15
#define QUAD_16 16
#define QUAD_17 17
#define QUAD_18 18
#define QUAD_19 19

vec2 getBlockQuadPosition(int blockID, int i)
{
	if(i == QUAD_0)  return vec2(1.0, 1.0) + vec2(0.0,  24.0) + vec2(34.0 * float(blockID), 0.0);
	if(i == QUAD_1)  return vec2(1.0, 1.0) + vec2(8.0,  24.0) + vec2(34.0 * float(blockID), 0.0);
	if(i == QUAD_2)  return vec2(1.0, 1.0) + vec2(16.0, 24.0) + vec2(34.0 * float(blockID), 0.0);
	if(i == QUAD_3)  return vec2(1.0, 1.0) + vec2(24.0, 24.0) + vec2(34.0 * float(blockID), 0.0);
	if(i == QUAD_4)  return vec2(1.0, 1.0) + vec2(0.0,  16.0) + vec2(34.0 * float(blockID), 0.0);
	if(i == QUAD_5)  return vec2(1.0, 1.0) + vec2(8.0,  16.0) + vec2(34.0 * float(blockID), 0.0);
	if(i == QUAD_6)  return vec2(1.0, 1.0) + vec2(16.0, 16.0) + vec2(34.0 * float(blockID), 0.0);
	if(i == QUAD_7)  return vec2(1.0, 1.0) + vec2(24.0, 16.0) + vec2(34.0 * float(blockID), 0.0);
	if(i == QUAD_8)  return vec2(1.0, 1.0) + vec2(0.0,  8.0 ) + vec2(34.0 * float(blockID), 0.0);
	if(i == QUAD_9)  return vec2(1.0, 1.0) + vec2(8.0,  8.0 ) + vec2(34.0 * float(blockID), 0.0);
	if(i == QUAD_10) return vec2(1.0, 1.0) + vec2(16.0, 8.0 ) + vec2(34.0 * float(blockID), 0.0);
	if(i == QUAD_11) return vec2(1.0, 1.0) + vec2(24.0, 8.0 ) + vec2(34.0 * float(blockID), 0.0);
	if(i == QUAD_12) return vec2(1.0, 1.0) + vec2(0.0,  0.0 ) + vec2(34.0 * float(blockID), 0.0);
	if(i == QUAD_13) return vec2(1.0, 1.0) + vec2(8.0,  0.0 ) + vec2(34.0 * float(blockID), 0.0);
	if(i == QUAD_14) return vec2(1.0, 1.0) + vec2(16.0, 0.0 ) + vec2(34.0 * float(blockID), 0.0);
	if(i == QUAD_15) return vec2(1.0, 1.0) + vec2(24.0, 0.0 ) + vec2(34.0 * float(blockID), 0.0);
	if(i == QUAD_16) return vec2(1.0, 1.0) + vec2(0.0, 40.0) + vec2(34.0 * float(blockID), 0.0);
	if(i == QUAD_17) return vec2(1.0, 1.0) + vec2(8.0, 40.0) + vec2(34.0 * float(blockID), 0.0);
	if(i == QUAD_18) return vec2(1.0, 1.0) + vec2(0.0, 32.0) + vec2(34.0 * float(blockID), 0.0);
	if(i == QUAD_19) return vec2(1.0, 1.0) + vec2(8.0, 32.0) + vec2(34.0 * float(blockID), 0.0);
	return vec2(0.0);
}

void main()
{
	// Block atlas texel size
	vec2 texelSize = vec2(1.0 / 2048.0);

	// Sub-block texture coordinate
	vec2 blockSubTexCoord = mod(v_TexCoord, 1.0 / CHUNK_BLOCKS) / (1.0 / CHUNK_BLOCKS);
	vec2 quadSubTexCoord = mod(v_TexCoord, 0.5 / CHUNK_BLOCKS) / (0.5 / CHUNK_BLOCKS);
	
	// Get block ID
	int blockID = int(texture(u_TileMap, v_TexCoord).r);
	int leftBlockID		= int(texture(u_TileMap, v_TexCoord - vec2(1.0 / CHUNK_BLOCKS, 0.0)).r);
	int rightBlockID	= int(texture(u_TileMap, v_TexCoord + vec2(1.0 / CHUNK_BLOCKS, 0.0)).r);
	int bottomBlockID	= int(texture(u_TileMap, v_TexCoord - vec2(0.0, 1.0 / CHUNK_BLOCKS)).r);
	int topBlockID		= int(texture(u_TileMap, v_TexCoord + vec2(0.0, 1.0 / CHUNK_BLOCKS)).r);
	int topLeftBlockID	= int(texture(u_TileMap, v_TexCoord + vec2(-1.0 / CHUNK_BLOCKS, 1.0 / CHUNK_BLOCKS)).r);
	int topRightBlockID	= int(texture(u_TileMap, v_TexCoord + vec2(1.0 / CHUNK_BLOCKS, 1.0 / CHUNK_BLOCKS)).r);
	int bottomLeftBlockID	= int(texture(u_TileMap, v_TexCoord + vec2(-1.0 / CHUNK_BLOCKS, -1.0 / CHUNK_BLOCKS)).r);
	int bottomRightBlockID	= int(texture(u_TileMap, v_TexCoord + vec2(1.0 / CHUNK_BLOCKS, -1.0 / CHUNK_BLOCKS)).r);
	
	vec4 color = vec4(0.0);
	int quad = -1;
	int maxBlock = blockID;
	if (blockSubTexCoord.x < 0.5)
	{
		if (blockSubTexCoord.y < 0.5)
		{
			vec4 botColor = blockID == leftBlockID ? vec4(0.0) : vec4(texture(u_BlockAtlas, (getBlockQuadPosition(leftBlockID, QUAD_11) + quadSubTexCoord * 8.0) * texelSize));
			vec4 topColor = blockID == bottomBlockID ? vec4(0.0) : vec4(texture(u_BlockAtlas, (getBlockQuadPosition(bottomBlockID, QUAD_1) + quadSubTexCoord * 8.0) * texelSize));
			
			maxBlock = max(maxBlock, leftBlockID);
			maxBlock = max(maxBlock, bottomBlockID);
			maxBlock = max(maxBlock, bottomLeftBlockID);
			
			vec4 color3 = vec4(0.0);
			if(bottomLeftBlockID > blockID && bottomLeftBlockID > leftBlockID && bottomLeftBlockID > bottomBlockID) {
				color3 = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(bottomLeftBlockID, QUAD_3) + quadSubTexCoord * 8.0) * texelSize));
			}
			else if(bottomLeftBlockID >= blockID && leftBlockID == bottomBlockID && blockID != bottomBlockID) {
				color3 = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(bottomBlockID, QUAD_18) + quadSubTexCoord * 8.0) * texelSize));
			}
			if(leftBlockID > bottomBlockID) { vec4 tmp = botColor; botColor = topColor; topColor = tmp;  }			
			color = mix(mix(botColor, topColor, topColor.a), color3, color3.a);
			quad = QUAD_9;
		}
		else
		{
			vec4 botColor = blockID == leftBlockID ? vec4(0.0) : vec4(texture(u_BlockAtlas, (getBlockQuadPosition(leftBlockID, QUAD_7) + quadSubTexCoord * 8.0) * texelSize));
			vec4 topColor = blockID == topBlockID ? vec4(0.0) : vec4(texture(u_BlockAtlas, (getBlockQuadPosition(topBlockID, QUAD_13) + quadSubTexCoord * 8.0) * texelSize));
			
			maxBlock = max(maxBlock, leftBlockID);
			maxBlock = max(maxBlock, topBlockID);
			maxBlock = max(maxBlock, topLeftBlockID);
			
			vec4 color3 = vec4(0.0);
			if(topLeftBlockID > blockID && topLeftBlockID > leftBlockID && topLeftBlockID > topBlockID) {
				color3 = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(topLeftBlockID, QUAD_15) + quadSubTexCoord * 8.0) * texelSize));
			}
			else if(topLeftBlockID >= blockID && leftBlockID == topBlockID && blockID != topBlockID) {
				color3 = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(topBlockID, QUAD_16) + quadSubTexCoord * 8.0) * texelSize));
			}
			if(leftBlockID > topBlockID) { vec4 tmp = botColor; botColor = topColor; topColor = tmp;  }	
			color = mix(mix(botColor, topColor, topColor.a), color3, color3.a);
			quad = QUAD_5;
		}
	}
	else
	{
		if (blockSubTexCoord.y < 0.5)
		{
			vec4 botColor = blockID == rightBlockID ? vec4(0.0) : vec4(texture(u_BlockAtlas, (getBlockQuadPosition(rightBlockID, QUAD_8 ) + quadSubTexCoord * 8.0) * texelSize));
			vec4 topColor = blockID == bottomBlockID ? vec4(0.0) : vec4(texture(u_BlockAtlas, (getBlockQuadPosition(bottomBlockID, QUAD_2) + quadSubTexCoord * 8.0) * texelSize));
			
			maxBlock = max(maxBlock, rightBlockID);
			maxBlock = max(maxBlock, bottomBlockID);
			maxBlock = max(maxBlock, bottomRightBlockID);
			
			vec4 color3 = vec4(0.0);
			if(bottomRightBlockID > blockID && bottomRightBlockID > rightBlockID && bottomRightBlockID > bottomBlockID) {
				color3 = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(bottomRightBlockID , QUAD_0) + quadSubTexCoord * 8.0) * texelSize));
			}
			else if(bottomRightBlockID >= blockID && rightBlockID == bottomBlockID && blockID != bottomBlockID) {
				color3 = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(bottomBlockID , QUAD_19) + quadSubTexCoord * 8.0) * texelSize));
			}
			if(rightBlockID > bottomBlockID) { vec4 tmp = botColor; botColor = topColor; topColor = tmp;  }	
			color = mix(mix(botColor, topColor, topColor.a), color3, color3.a);
			quad = QUAD_10;
		}
		else
		{
			vec4 botColor = blockID == rightBlockID ? vec4(0.0) : vec4(texture(u_BlockAtlas, (getBlockQuadPosition(rightBlockID, QUAD_4 ) + quadSubTexCoord * 8.0) * texelSize));
			vec4 topColor = blockID == topBlockID ? vec4(0.0) : vec4(texture(u_BlockAtlas, (getBlockQuadPosition(topBlockID, QUAD_14) + quadSubTexCoord * 8.0) * texelSize));
			vec4 color3 = vec4(0.0);
			
			maxBlock = max(maxBlock, rightBlockID);
			maxBlock = max(maxBlock, topBlockID);
			maxBlock = max(maxBlock, topRightBlockID);
			
			if(topRightBlockID > blockID && topRightBlockID > rightBlockID && topRightBlockID > topBlockID) {
				color3 = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(topRightBlockID , QUAD_12) + quadSubTexCoord * 8.0) * texelSize));
			}
			else if(topRightBlockID >= blockID && rightBlockID == topBlockID && blockID != topBlockID) {
				color3 = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(topBlockID , QUAD_17) + quadSubTexCoord * 8.0) * texelSize));
			}
			if(rightBlockID > topBlockID) { vec4 tmp = botColor; botColor = topColor; topColor = tmp;  }	
			color = mix(mix(botColor, topColor, topColor.a), color3, color3.a);
			quad = QUAD_6;
		}
	}
	
	vec4 blockColor = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(blockID, quad) + quadSubTexCoord * 8.0) * texelSize));
	
	if(maxBlock > blockID)
	{ vec4 tmp = blockColor; blockColor = color; color = tmp;  }
	
	out_FragColor = mix(color, blockColor, blockColor.a);
}