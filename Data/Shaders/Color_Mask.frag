in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform sampler2D u_Texture;
uniform vec4 u_Colors[3];

void main()
{
	vec4 mask = texture(u_Texture, v_TexCoord);
	out_FragColor = u_Colors[0] * mask[0];
	out_FragColor = mix(out_FragColor, u_Colors[1], mask[1]);
	out_FragColor = mix(out_FragColor, u_Colors[2], mask[2]);
	out_FragColor.a = mask.a;
}