in vec2 v_TexCoord;
out vec4 out_FragColor;

#define BLOCK_COUNT 10

uniform vec4 u_BlockColors[BLOCK_COUNT];
uniform sampler2D u_BlockTexture;

void main()
{
	uvec4 blockIDs = uvec4(texture(u_BlockTexture, v_TexCoord) * 255.5);
	uint blockID = uint(mix(mix(blockIDs[0], blockIDs[1], blockIDs[1] != 0U), blockIDs[2], blockIDs[2] != 0U));
	out_FragColor = u_BlockColors[blockID];
}