in vec2 v_TexCoord;
out vec4 out_FragColor;

#define PI 3.14159265359

uniform sampler2D u_Texture;
uniform vec2 u_Resolution;
uniform float u_SoftShadows;
uniform vec3 u_Color;

// Sample from the distance map
float sample(vec2 coord, float r)
{
  return step(r, texture(u_Texture, coord).r);
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
	
	// The center tex coord, which gives us hard shadows
	float center = sample(vec2(tc.x, tc.y), r);        
	
	// We multiply the blur amount by our distance from center
	// This leads to more blurriness as the shadow "fades away"
	float blur = (1.0 / u_Resolution.x)  * smoothstep(0., 1., r); 
	
	// Now we use a simple gaussian blur
	float sum = 0.0;
	
	sum += sample(vec2(tc.x - 4.0*blur, tc.y), r) * 0.05;
	sum += sample(vec2(tc.x - 3.0*blur, tc.y), r) * 0.09;
	sum += sample(vec2(tc.x - 2.0*blur, tc.y), r) * 0.12;
	sum += sample(vec2(tc.x - 1.0*blur, tc.y), r) * 0.15;
	
	sum += center * 0.16;
	
	sum += sample(vec2(tc.x + 1.0*blur, tc.y), r) * 0.15;
	sum += sample(vec2(tc.x + 2.0*blur, tc.y), r) * 0.12;
	sum += sample(vec2(tc.x + 3.0*blur, tc.y), r) * 0.09;
	sum += sample(vec2(tc.x + 4.0*blur, tc.y), r) * 0.05;
	
	// 1.0 -> in light, 0.0 -> in shadow
	float lit = mix(center, sum, u_SoftShadows);
 	
	// Multiply the summed amount by our distance, which gives us a radial falloff
	// then multiply by vertex (light) color  
	out_FragColor = vec4(u_Color, lit * smoothstep(1.0, 0.0, r));
}