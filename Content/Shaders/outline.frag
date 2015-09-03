#version 120

varying vec2 texCoord;
uniform sampler2D texture;
uniform vec2 resolution;
uniform vec3 color;
uniform float radius;
uniform float step;

float func(float x)
{
	return -4.0f * pow(x - 0.5f, 2.0f) + 1.0f;
}

void main()
{
	// Setup loop vars
	vec2 dtvec = 1.0f/resolution;
	float acc = 0.0f;
	float i = 0.0f;
	
	// Sample outline accumulator
	for(float y = -radius; y <= radius; y += step)
	{
		for(float x = -radius; x <= radius; x += step)
		{
			vec2 pos = vec2(x, y);
			if(length(pos) < radius)
			{
				acc += texture2D(texture, texCoord + dtvec*pos).a;
				i += 1.0f;
			}
		}
	}
	
	// Set frag color
	gl_FragColor = mix(texture2D(texture, texCoord), vec4(color, 1.0f), func(acc/i));
}