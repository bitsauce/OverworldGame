in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform sampler2D u_Texture;

/*void main()
{
	float factor = v_TexCoord.y - u_OffsetY;
	if(factor < 0.1) factor = 0.0;
	
	float sum = 0.0;
	for(int i = -5; i <= 5; i++)
	{
		sum += texture(u_Texture, v_TexCoord + vec2(float(i) / u_Width, float(i) / u_Height) * vec2(cos(u_Direction), sin(u_Direction)) * factor).a;
	}
	sum /= 10.0;
	
	out_FragColor = vec4(vec3(sum), 1.0);
}*/

void main()
{
	out_FragColor = vec4(vec3(texture(u_Texture, v_TexCoord).a), 1.0);
}