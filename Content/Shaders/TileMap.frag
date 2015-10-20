// Current assembly line count: 463 lines
// Current assembly file size: 19497 bytes

in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform usampler2D u_SortedBlockTexture;
uniform usampler2D u_SortedQuadTexture;
uniform sampler2D u_BlockAtlas;
uniform vec2 u_QuadUVs[20];

void main()
{
	// Block atlas texel size
	const vec2 texelSize = vec2(1.0 / 2048.0);
	vec2 sortedTexureSize = textureSize(u_SortedBlockTexture, 0);

	// Sub-block texture coordinate
	vec2 blockSubTexCoord = mod(v_TexCoord, 1.0 / sortedTexureSize.x) / (1.0 / sortedTexureSize.y);
	vec2 quadSubTexCoord = mod(v_TexCoord, 0.5 / sortedTexureSize.x) / (0.5 / sortedTexureSize.y);
	
	// Get quad index
	int quadIndex = int(dot(vec2(greaterThanEqual(blockSubTexCoord, vec2(0.5))), vec2(1.0, 2.0)));

	// Get block ids and quad ids
    uint blocks = texture(u_SortedBlockTexture, v_TexCoord)[quadIndex];
	uvec4 blockIDs = uvec4((blocks >> 24U) & 0xFFU, (blocks >> 16U) & 0xFFU, (blocks >> 8U) & 0xFFU, blocks & 0xFFU);
    uint quads = texture(u_SortedQuadTexture, v_TexCoord)[quadIndex];
	uvec4 quadIDs = uvec4((quads >> 24U) & 0xFFU, (quads >> 16U) & 0xFFU, (quads >> 8U) & 0xFFU, quads & 0xFFU);

    // Blends together colors from tile edges, corners and fill
	out_FragColor = texture(u_BlockAtlas, ((u_QuadUVs[quadIDs[0]] + vec2(34.0 * float(blockIDs[0]), 0.0)) + quadSubTexCoord * 8.0) * texelSize);
    for (int i = 1; i < 4; ++i)
	{
        vec4 src = float(blockIDs[i] != blockIDs[i - 1]) * texture(u_BlockAtlas, ((u_QuadUVs[quadIDs[i]] + vec2(34.0 * float(blockIDs[i]), 0.0)) + quadSubTexCoord * 8.0) * texelSize);
		out_FragColor += src - src.a * out_FragColor;
    }
}