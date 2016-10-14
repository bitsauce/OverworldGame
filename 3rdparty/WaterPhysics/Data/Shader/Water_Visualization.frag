in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform sampler2D u_Texture;

void main()
{
	texture(u_Texture, v_TexCoord).r;
	out_FragColor = texture(u_Texture, v_TexCoord);
}