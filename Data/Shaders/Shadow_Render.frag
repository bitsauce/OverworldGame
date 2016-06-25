in vec2 v_TexCoord;
out vec4 out_FragColor;

#define PI 3.14159265359

uniform sampler2D u_Texture;
uniform sampler2D u_OccluderTexture;
uniform vec2 u_Resolution;
uniform vec3 u_Color;

// Sample from the distance map
float sample(vec2 coord, float r)
{
	// Get occluder position
	float f = texture(u_Texture, coord).r;
	float dt = r - f; // Distance from this fragment to the occluder
	
	// If this fragment is being occluded
	if(f < 1.0 && dt > 0.0)
	{
		// And is inside the occluder
		if(texture(u_OccluderTexture, v_TexCoord).a > 0.0)
		{
			// Then, make the light fade into the occluder
			float a = 1.0 - r;
			return smoothstep(smoothstep(1.0, 0.0, f), 0.0, clamp(min(dt, a) / a, 0.0, 1.0));
		}
		else
		{
			// If not, return a darker value for the gradient
			return smoothstep(1.0, 0.0, pow(r, 0.5));
		}
	}

	// Else, return a simple gradient value
	return smoothstep(1.0, 0.0, r);
}

void main()
{
	// Rectangular to polar
	vec2 norm = v_TexCoord.st * 2.0 - 1.0;
	float theta = atan(norm.y, norm.x);
	float r = length(norm);	
	float coord = (theta + PI) / (2.0 * PI);
	
	// The tex coord to sample our 1D lookup texture	
	// always 0.0 on y axis
	vec2 tc = vec2(coord, 0.0);
	
	// Sample the coordinate and set the fragment color value
	out_FragColor = vec4(u_Color, sample(vec2(tc.x, tc.y), r));
}