in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform sampler2D u_Texture;

void main()
{
	vec4 r = texture(u_Texture, v_TexCoord - vec2(0.0, 1.0 / textureSize(u_Texture, 0).y));
	out_FragColor = r;
}