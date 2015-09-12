in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform usampler2D u_TileMap;
uniform sampler2D u_BlockAtlas;

#define BLOCK_PX 16.0
#define CHUNK_BLOCKS 32.0
#define CHUNK_PX (CHUNK_BLOCKS * BLOCK_PX)

void main()
{

	float tx = 1.0 / 2048.0,
		ty = 1.0 / 2048.0;
		
	
	vec2 v = vec2(ivec2(v_TexCoord * CHUNK_BLOCKS)) / CHUNK_BLOCKS;
	
	uint i = texture(u_TileMap, v_TexCoord).r;
	if(i == uint(0))
	{
		out_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	}
	else if(i == uint(1))
	{
		out_FragColor = vec4(0.0, 1.0, 0.0, 1.0);
	}
	else if(i == uint(2))
	{
		out_FragColor = vec4(0.0, 0.0, 1.0, 1.0);
	}
	
	//out_FragColor = vec4(vec3(length(v)), 1.0);
	
	//out_FragColor = vec4(texture(u_BlockAtlas, v_TexCoord));
}