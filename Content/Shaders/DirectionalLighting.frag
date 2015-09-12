in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform sampler2D u_Texture;
uniform float u_OffsetY;
uniform float u_Direction;
uniform float u_Width;
uniform float u_Height;

void main()
{
	float factor = v_TexCoord.y - u_OffsetY;
	if(factor < 0.1) factor = 0.0;
	
	float sum = 0.0;
	for(int i = -5; i <= 5; i++)
	{
		sum += texture(u_Texture, v_TexCoord + vec2(float(i) / u_Width, float(i) / u_Height) * vec2(cos(u_Direction), sin(u_Direction)) * factor).r;
	}
	sum /= 10.0;
	
	out_FragColor = vec4(vec3(sum), 1.0);
	
	//out_FragColor = vec4(vec3(0.0), min(texture2D(u_Texture, v_TexCoord).a, min(texture2D(u_Texture, v_TexCoord + vec2(1.0/u_Width, 0.0)).a, min(texture2D(u_Texture, v_TexCoord - vec2(1.0/u_Width, 0.0)).a, min(texture2D(u_Texture, v_TexCoord + vec2(0.0, 1.0/u_Height)).a, texture2D(u_Texture, v_TexCoord - vec2(0.0, 1.0/u_Height)).a)))));
}