in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform sampler2D u_Texture;

uniform float u_Exposure;
uniform float u_Decay;
uniform float u_Density;
uniform vec2 u_LightPos;

#define NUM_SAMPLES 100

void main()
{
	// Apply god rays
	vec2 pos = v_TexCoord;
	vec2 delta = ((pos - u_LightPos) / float(NUM_SAMPLES)) * u_Density;
	
	float falloff = 1.0;
	out_FragColor = vec4(0.0);
	for(int i = 0; i < NUM_SAMPLES; i++)
	{
		pos -= delta;
		out_FragColor += texture(u_Texture, pos) * falloff;
		falloff *= u_Decay;
	}
	
	out_FragColor *= u_Exposure;
}