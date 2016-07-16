in vec2 v_TexCoord;
out vec4 out_FragColor;

#define BLOCK_EMPTY 0U
#define BLOCK_GRASS 1
#define BLOCK_DIRT 2
#define BLOCK_DIRT_BACK 3
#define BLOCK_STONE 4
#define BLOCK_STONE_BACK 5
#define BLOCK_COUNT 6

uniform sampler2D u_BlockTexture;
uniform vec4 u_BlockColors[BLOCK_COUNT];

void main()
{
	uvec3 blockIDs = uvec3(texture(u_BlockTexture, v_TexCoord).rgb * 255.5);
	out_FragColor = u_BlockColors[BLOCK_EMPTY];
	for(uint i = 0U; i < 3U; i++)
	{
		if(blockIDs[i] != BLOCK_EMPTY)
		{
			out_FragColor = u_BlockColors[blockIDs[i]];
		}
	}
}