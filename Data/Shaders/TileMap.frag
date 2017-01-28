in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform usampler2D u_SortedBlockTexture;
uniform usampler2D u_SortedQuadTexture;
uniform sampler2D u_BlockAtlas;
uniform sampler2D u_BlockData;
uniform sampler2D u_BlockSubID;
uniform vec2 u_QuadUVs[20];

void main()
{
	// Block atlas texel size
	vec2 texelSize = 1.0 / textureSize(u_BlockAtlas, 0);
	vec2 blockDataTextureSize = 1.0 / textureSize(u_BlockData, 0);
	vec2 sortedTexureSize = textureSize(u_SortedBlockTexture, 0);

	// Sub-block texture coordinate
	vec2 blockSubTexCoord = mod(v_TexCoord, 1.0 / sortedTexureSize) / (1.0 / sortedTexureSize);
	vec2 quadSubTexCoord = mod(v_TexCoord, 0.5 / sortedTexureSize) / (0.5 / sortedTexureSize);
	
	// Get quad index
	int quadIndex = int(dot(vec2(greaterThanEqual(blockSubTexCoord, vec2(0.5))), vec2(1.0, 2.0)));

	// Get block ids and quad ids
    uint blocks = texture(u_SortedBlockTexture, v_TexCoord)[quadIndex];
	uvec4 blockVecIDs = uvec4((blocks >> 24U) & 0xFFU, (blocks >> 16U) & 0xFFU, (blocks >> 8U) & 0xFFU, blocks & 0xFFU);
	uvec4 blockIDs = blockVecIDs / 16U;
	uvec4 blockSubIDs = blockVecIDs - blockIDs * 16U;
    uint quads = texture(u_SortedQuadTexture, v_TexCoord)[quadIndex];
	uvec4 quadIDs = uvec4((quads >> 24U) & 0xFFU, (quads >> 16U) & 0xFFU, (quads >> 8U) & 0xFFU, quads & 0xFFU);

    // Blends together colors from edges, corners and fill
	vec4 blockUVs = texture(u_BlockData, (vec2(blockIDs[0], 1.0 + float(blockSubIDs[0])) + 0.5) * blockDataTextureSize);
	vec4 blockData = texture(u_BlockData, (vec2(blockIDs[0], 0.0) + 0.5) * blockDataTextureSize);
	vec2 blockAltasPosition = vec2(float(uint(blockUVs[0] * 255.5) | (uint(blockUVs[1] * 255.5) << 8U)), float(uint(blockUVs[2] * 255.5) | (uint(blockUVs[3] * 255.5) << 8U)));
	
	out_FragColor = texture(u_BlockAtlas, (blockAltasPosition + u_QuadUVs[quadIDs[0]] + quadSubTexCoord * 8.0) * texelSize);
    for (int i = 1; i < 4; ++i)
	{
		blockUVs = texture(u_BlockData, (vec2(blockIDs[i], 1.0 + float(blockSubIDs[i])) + 0.5) * blockDataTextureSize);
		blockData = texture(u_BlockData, (vec2(blockIDs[i], 0.0) + 0.5) * blockDataTextureSize);
		blockAltasPosition = vec2(float(uint(blockUVs[0] * 255.5) | (uint(blockUVs[1] * 255.5) << 8U)), float(uint(blockUVs[2] * 255.5) | (uint(blockUVs[3] * 255.5) << 8U)));

        vec4 src = float(blockVecIDs[i] != blockVecIDs[i - 1]) * texture(u_BlockAtlas, (blockAltasPosition + u_QuadUVs[quadIDs[i]] + quadSubTexCoord * 8.0) * texelSize);
		out_FragColor += src - src.a * out_FragColor;
    }
}