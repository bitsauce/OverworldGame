in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform sampler2D u_WaterTexture;

void main()
{
	out_FragColor = vec4(0.0, 0.0, min(texture(u_WaterTexture, v_TexCoord).r * 4.0, 1.0), 1.0);
}