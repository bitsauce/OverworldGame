in vec2 u_TexCoord;

uniform sampler2D u_Texture;
uniform vec2 u_Resolution;
uniform vec3 u_Color;
uniform float u_Radius;
uniform float u_Step;

float func(float x)
{
	return -4.0f * pow(x - 0.5f, 2.0f) + 1.0f;
}

void main()
{
	// Setup loop vars
	vec2 dtvec = 1.0f/u_Resolution;
	float acc = 0.0f;
	float i = 0.0f;
	
	// Sample outline accumulator
	for(float y = -u_Radius; y <= u_Radius; y += u_Step)
	{
		for(float x = -u_Radius; x <= u_Radius; x += u_Step)
		{
			vec2 pos = vec2(x, y);
			if(length(pos) < u_Radius)
			{
				acc += texture(u_Texture, u_TexCoord + dtvec*pos).a;
				i += 1.0f;
			}
		}
	}
	
	// Set frag u_Color
	gl_Fragu_Color = mix(texture(u_Texture, u_TexCoord), vec4(u_Color, 1.0f), func(acc/i));
}