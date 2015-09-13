in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform usampler2D u_TileMap;
uniform sampler2D u_BlockAtlas;

#define BLOCK_PX 16.0
#define CHUNK_BLOCKS 32.0
#define CHUNK_PX (CHUNK_BLOCKS * BLOCK_PX)

void main()
{
	//float btx = 1.0 / 40.0, bty = 1.0 / 24.0;
	//vec2 v = vec2(ivec2(v_TexCoord * CHUNK_BLOCKS)) / CHUNK_BLOCKS;
	
	vec2 texCoord = mod(v_TexCoord, 1.0 / CHUNK_BLOCKS) / (1.0 / CHUNK_BLOCKS);
	
	vec2 texelSize = vec2(1.0 / 2048.0);
	int blockID = int(texture(u_TileMap, v_TexCoord).r) - 2;
	vec2 blockTexCoord = vec2(texelSize) * vec2(5.0, 4.0 + 42.0 * float(blockID));
	
	out_FragColor = vec4(texture(u_BlockAtlas, blockTexCoord + texCoord * texelSize * 16.0));
	//out_FragColor = vec4(vec3(length(texCoord)), 1.0);
}