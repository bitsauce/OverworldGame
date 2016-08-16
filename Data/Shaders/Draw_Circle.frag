in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform vec4 u_Color;
uniform uint u_DrawOutline;

void main()
{
	vec2 pos = v_TexCoord * 2.0 - 1.0;
	vec4 color = u_Color;
	color.a *= 0.25;
	if(u_DrawOutline > 0U)
	{
		out_FragColor = vec4(mix(mix(color, vec4(1.0), length(pos) > 0.98), vec4(0.0), length(pos) > 1.0));
	}
	else
	{
		out_FragColor = vec4(mix(color, vec4(0.0), length(pos) > 1.0));
	}
}