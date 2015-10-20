in vec2 v_TexCoord;
out uvec4 out_FragColor;

uniform usampler2D u_BlockTexture;

void main()
{
	out_FragColor = texture(u_BlockTexture, v_TexCoord);
}