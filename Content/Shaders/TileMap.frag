// Current assembly line count: 463 lines
// Current assembly file size: 19497 bytes

in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform usampler2D u_TileMap;
uniform sampler2D u_BlockAtlas;

#define BLOCK_PX 16.0
#define CHUNK_BLOCKS 34.0

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

struct BlockColorID
{
	int blockID;
	vec4 color;
};

void main()
{
	// Block atlas texel size
	vec2 texelSize = vec2(1.0 / 2048.0);

	// Sub-block texture coordinate
	vec2 blockSubTexCoord = mod(v_TexCoord, 1.0 / CHUNK_BLOCKS) / (1.0 / CHUNK_BLOCKS);
	vec2 quadSubTexCoord = mod(v_TexCoord, 0.5 / CHUNK_BLOCKS) / (0.5 / CHUNK_BLOCKS);
	
	// Get block ID
	int blockID = int(texture(u_TileMap, v_TexCoord).r);
	
	// Get block quad color
	BlockColorID blockColorIDs[5];
	int idx = 0;
	if (blockSubTexCoord.x < 0.5)
	{
		int leftBlockID = int(texture(u_TileMap, v_TexCoord - vec2(1.0 / CHUNK_BLOCKS, 0.0)).r);
		
		if (blockSubTexCoord.y < 0.5)
		{
			int bottomBlockID = int(texture(u_TileMap, v_TexCoord - vec2(0.0, 1.0 / CHUNK_BLOCKS)).r);
			int bottomLeftBlockID = int(texture(u_TileMap, v_TexCoord + vec2(-1.0 / CHUNK_BLOCKS, -1.0 / CHUNK_BLOCKS)).r);
			
			if(blockID != leftBlockID)
			{
				blockColorIDs[idx].blockID = leftBlockID;
				blockColorIDs[idx].color = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(leftBlockID, QUAD_11) + quadSubTexCoord * 8.0) * texelSize));
				idx++;
			}
			
			if(blockID != bottomBlockID)
			{
				blockColorIDs[idx].blockID = bottomBlockID;
				blockColorIDs[idx].color = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(bottomBlockID, QUAD_1) + quadSubTexCoord * 8.0) * texelSize));
				idx++;
			}
			
			if(bottomLeftBlockID > blockID && bottomLeftBlockID > leftBlockID && bottomLeftBlockID > bottomBlockID)
			{
				blockColorIDs[idx].blockID = bottomLeftBlockID;
				blockColorIDs[idx].color = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(bottomLeftBlockID, QUAD_3) + quadSubTexCoord * 8.0) * texelSize));
				idx++;
			}
			
			if(bottomLeftBlockID >= blockID && leftBlockID == bottomBlockID && blockID != bottomBlockID)
			{
				blockColorIDs[idx].blockID = bottomBlockID;
				blockColorIDs[idx].color = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(bottomBlockID, QUAD_18) + quadSubTexCoord * 8.0) * texelSize));
				idx++;
			}
			
			blockColorIDs[idx].blockID = blockID;
			blockColorIDs[idx].color = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(blockID, QUAD_9) + quadSubTexCoord * 8.0) * texelSize));
			idx++;
		}
		else
		{
			int topBlockID = int(texture(u_TileMap, v_TexCoord + vec2(0.0, 1.0 / CHUNK_BLOCKS)).r);
			int topLeftBlockID	= int(texture(u_TileMap, v_TexCoord + vec2(-1.0 / CHUNK_BLOCKS, 1.0 / CHUNK_BLOCKS)).r);
			
			if(blockID != leftBlockID)
			{
				blockColorIDs[idx].blockID = leftBlockID;
				blockColorIDs[idx].color = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(leftBlockID, QUAD_7) + quadSubTexCoord * 8.0) * texelSize));
				idx++;
			}
			
			if(blockID != topBlockID)
			{
				blockColorIDs[idx].blockID = topBlockID;
				blockColorIDs[idx].color = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(topBlockID, QUAD_13) + quadSubTexCoord * 8.0) * texelSize));
				idx++;
			}
			
			if(topLeftBlockID > blockID && topLeftBlockID > leftBlockID && topLeftBlockID > topBlockID)
			{
				blockColorIDs[idx].blockID = topLeftBlockID;
				blockColorIDs[idx].color = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(topLeftBlockID, QUAD_15) + quadSubTexCoord * 8.0) * texelSize));
				idx++;
			}
			
			if(topLeftBlockID >= blockID && leftBlockID == topBlockID && blockID != topBlockID)
			{
				blockColorIDs[idx].blockID = topBlockID;
				blockColorIDs[idx].color = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(topBlockID, QUAD_16) + quadSubTexCoord * 8.0) * texelSize));
				idx++;
			}
			
			blockColorIDs[idx].blockID = blockID;
			blockColorIDs[idx].color = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(blockID, QUAD_5) + quadSubTexCoord * 8.0) * texelSize));
			idx++;
		}
	}
	else
	{
		int rightBlockID = int(texture(u_TileMap, v_TexCoord + vec2(1.0 / CHUNK_BLOCKS, 0.0)).r);
		
		if (blockSubTexCoord.y < 0.5)
		{
			int bottomBlockID = int(texture(u_TileMap, v_TexCoord - vec2(0.0, 1.0 / CHUNK_BLOCKS)).r);
			int bottomRightBlockID	= int(texture(u_TileMap, v_TexCoord + vec2(1.0 / CHUNK_BLOCKS, -1.0 / CHUNK_BLOCKS)).r);
			
			if(blockID != rightBlockID)
			{
				blockColorIDs[idx].blockID = rightBlockID;
				blockColorIDs[idx].color = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(rightBlockID, QUAD_8) + quadSubTexCoord * 8.0) * texelSize));
				idx++;
			}
			
			if(blockID != bottomBlockID)
			{
				blockColorIDs[idx].blockID = bottomBlockID;
				blockColorIDs[idx].color = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(bottomBlockID, QUAD_2) + quadSubTexCoord * 8.0) * texelSize));
				idx++;
			}
			
			if(bottomRightBlockID > blockID && bottomRightBlockID > rightBlockID && bottomRightBlockID > bottomBlockID)
			{
				blockColorIDs[idx].blockID = bottomRightBlockID;
				blockColorIDs[idx].color = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(bottomRightBlockID, QUAD_0) + quadSubTexCoord * 8.0) * texelSize));
				idx++;
			}
			
			if(bottomRightBlockID >= blockID && rightBlockID == bottomBlockID && blockID != bottomBlockID)
			{
				blockColorIDs[idx].blockID = bottomBlockID;
				blockColorIDs[idx].color = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(bottomBlockID, QUAD_19) + quadSubTexCoord * 8.0) * texelSize));
				idx++;
			}
			
			blockColorIDs[idx].blockID = blockID;
			blockColorIDs[idx].color = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(blockID, QUAD_10) + quadSubTexCoord * 8.0) * texelSize));
			idx++;
		}
		else
		{
			int topBlockID = int(texture(u_TileMap, v_TexCoord + vec2(0.0, 1.0 / CHUNK_BLOCKS)).r);
			int topRightBlockID	= int(texture(u_TileMap, v_TexCoord + vec2(1.0 / CHUNK_BLOCKS, 1.0 / CHUNK_BLOCKS)).r);
			
			if(blockID != rightBlockID)
			{
				blockColorIDs[idx].blockID = rightBlockID;
				blockColorIDs[idx].color = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(rightBlockID, QUAD_4) + quadSubTexCoord * 8.0) * texelSize));
				idx++;
			}
			
			if(blockID != topBlockID)
			{
				blockColorIDs[idx].blockID = topBlockID;
				blockColorIDs[idx].color = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(topBlockID, QUAD_14) + quadSubTexCoord * 8.0) * texelSize));
				idx++;
			}
			
			if(topRightBlockID > blockID && topRightBlockID > rightBlockID && topRightBlockID > topBlockID)
			{
				blockColorIDs[idx].blockID = topRightBlockID;
				blockColorIDs[idx].color = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(topRightBlockID, QUAD_12) + quadSubTexCoord * 8.0) * texelSize));
				idx++;
			}
			
			if(topRightBlockID >= blockID && rightBlockID == topBlockID && blockID != topBlockID)
			{
				blockColorIDs[idx].blockID = topBlockID;
				blockColorIDs[idx].color = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(topBlockID, QUAD_17) + quadSubTexCoord * 8.0) * texelSize));
				idx++;
			}
			
			blockColorIDs[idx].blockID = blockID;
			blockColorIDs[idx].color = vec4(texture(u_BlockAtlas, (getBlockQuadPosition(blockID, QUAD_6) + quadSubTexCoord * 8.0) * texelSize));
			idx++;
		}
	}
	
	// Perform insertion sort on block depth
	BlockColorID tmp;
	for(int i = 0; i < idx; ++i)
	{
		tmp = blockColorIDs[i];
		int j = i;
		while(j > 0 && blockColorIDs[j - 1].blockID > tmp.blockID)
		{
			blockColorIDs[j] = blockColorIDs[j - 1];
			--j;
		}
		blockColorIDs[j] = tmp;
	}
	
	// Overlap all colors
	vec4 outColor = blockColorIDs[0].color;
	for(int i = 1; i < idx; ++i)
	{
		vec4 color = blockColorIDs[i].color;
		outColor = mix(outColor, color, color.a);
	}
	out_FragColor = outColor;
}