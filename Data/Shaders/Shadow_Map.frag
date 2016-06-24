in vec2 v_TexCoord;
out vec4 out_FragColor;

#define PI 3.14159265359

// Alpha threshold for our occlusion map
#define THRESHOLD 0.0

uniform sampler2D u_Texture;
uniform vec2 u_Resolution;

// For debugging; use a constant value in final release
uniform float u_Scale;

void main()
{
	float distance = 1.0;
	for (float y = 0.0; y < u_Resolution.y; y += 1.0)
	{
		// Rectangular to polar filter
		vec2 norm = vec2(v_TexCoord.s, y / u_Resolution.y) * 2.0 - 1.0;
		float theta = PI * 1.5 + norm.x * PI; 
		float r = (1.0 + norm.y) * 0.5;
		
		// Coord which we will sample from occlude map
		vec2 coord = vec2(-r * sin(theta), r * cos(theta)) / 2.0 + 0.5;
		
		// Sample the occlusion map
		vec4 data = texture(u_Texture, coord);
		
		// The current distance is how far from the top we've come
		float dst = y / u_Resolution.y / u_Scale;
		
		// If we've hit an opaque fragment (occluder), then get new distance
		// if the new distance is below the current, then we'll use that for our ray
		float caster = data.a;
		if (caster > THRESHOLD) {
			distance = min(distance, dst);
		}
	} 
	out_FragColor = vec4(vec3(distance), 1.0);
}