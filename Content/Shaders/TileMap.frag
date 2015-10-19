// Current assembly line count: 463 lines
// Current assembly file size: 19497 bytes

in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform usampler2D u_SortedBlockTexture;
uniform usampler2D u_SortedQuadTexture;
uniform sampler2D u_BlockAtlas;
uniform vec2 u_QuadUVs[20];

#define CHUNK_BLOCKS 34.0
#define QUAD_0 0U
#define QUAD_1 1U
#define QUAD_2 2U
#define QUAD_3 3U
#define QUAD_4 4U
#define QUAD_5 5U
#define QUAD_6 6U
#define QUAD_7 7U
#define QUAD_8 8U
#define QUAD_9 9U
#define QUAD_10 10U
#define QUAD_11 11U
#define QUAD_12 12U
#define QUAD_13 13U
#define QUAD_14 14U
#define QUAD_15 15U
#define QUAD_16 16U
#define QUAD_17 17U
#define QUAD_18 18U
#define QUAD_19 19U

#pragma optionNV(unroll all)

void main()
{
	// Block atlas texel size
	const vec2 texelSize = vec2(1.0 / 2048.0);

	// Sub-block texture coordinate
	vec2 blockSubTexCoord = mod(v_TexCoord, 1.0 / CHUNK_BLOCKS) / (1.0 / CHUNK_BLOCKS);
	vec2 quadSubTexCoord = mod(v_TexCoord, 0.5 / CHUNK_BLOCKS) / (0.5 / CHUNK_BLOCKS);
	
	// Get quad index
	int quadIndex = int(dot(vec2(greaterThanEqual(blockSubTexCoord, vec2(0.5))), vec2(1.0, 2.0)));

	// Get block ids and quad ids
    uint blocks = texture(u_SortedBlockTexture, v_TexCoord)[quadIndex];
	uvec4 blockIDs = uvec4((blocks >> 24U) & 0xFFU, (blocks >> 16U) & 0xFFU, (blocks >> 8U) & 0xFFU, blocks & 0xFFU);
    uint quads = texture(u_SortedQuadTexture, v_TexCoord)[quadIndex];
	uvec4 quadIDs = uvec4((quads >> 24U) & 0xFFU, (quads >> 16U) & 0xFFU, (quads >> 8U) & 0xFFU, quads & 0xFFU);

    // Blends together colors from tile edges, corners and fill
	//vec4 col; // TODO: This method using alpha blend is highly inefficient. Figure out how to use premultiplied alpha instead.
	out_FragColor = texture(u_BlockAtlas, ((u_QuadUVs[quadIDs[0]] + vec2(34.0 * float(blockIDs[0]), 0.0)) + quadSubTexCoord * 8.0) * texelSize);
    for (int i = 1; i < 4; ++i)
	{
        vec4 src = float(blockIDs[i] != blockIDs[i - 1]) * texture(u_BlockAtlas, ((u_QuadUVs[quadIDs[i]] + vec2(34.0 * float(blockIDs[i]), 0.0)) + quadSubTexCoord * 8.0) * texelSize);
		out_FragColor += src - src.a * out_FragColor;

		//col.a = src.a + dst.a * (1.0 - src.a);
		//if(col.a != 0.0)
		//	col.rgb = (src.rgb * src.a +  dst.rgb *  dst.a * (1 - src.a)) / col.a;
		//else
		//	col.rgb = vec3(0.0);
		//dst = col;
    }
}