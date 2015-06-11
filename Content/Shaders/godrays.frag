varying vec2 vTexcoord;

uniform sampler2D texture;

uniform float exposure;
uniform float decay;
uniform float density;
uniform vec2 lightPos;

#define NUM_SAMPLES 100

void main()
{
	// Apply god rays
	vec2 pos = vTexcoord;
	vec2 delta = ((pos-lightPos)/float(NUM_SAMPLES))*density;
	
	float falloff = 1.0;
	gl_FragColor = vec4(0.0);
	for(int i = 0; i < NUM_SAMPLES; i++)
	{
		pos -= delta;
		gl_FragColor += texture2D(texture, pos)*falloff;
		falloff *= decay;
	}
	
	gl_FragColor *= exposure;
}