in vec2 v_TexCoord;
out vec4 out_FragColor;

#define PI 3.14159265359

// Alpha threshold for our occlusion map
#define THRESHOLD 0.0

uniform sampler2D u_Texture;
uniform vec2 u_Resolution;

void main()
{
	// Rectangular to polar filter
	vec2 norm = v_TexCoord.st * 2.0 - 1.0;
	float theta = PI * 1.5 + norm.x * PI; 
	float r = (1.0 + norm.y) * 0.5;
		
	// Coord which we will sample from occlude map
	vec2 coord = vec2(-r * sin(theta), r * cos(theta)) / 2.0 + 0.5;
		
	// Sample the occlusion map
	out_FragColor = vec4(vec3(texture(u_Texture, coord)[1] > 0.0 ? 0.0 : 1.0), 1.0);
}