in vec2 v_TexCoord;
out vec4 out_FragColor;

#define PI 3.14159265359

uniform sampler2D u_Texture;
uniform sampler2D u_OccluderTexture;
//uniform vec2 u_Resolution;
uniform vec3 u_Color;

//uniform float u_Falloff;
//uniform float u_SolidTransparency;

void main()
{
	// Rectangular to polar
	vec2 norm = v_TexCoord.st * 2.0 - 1.0;
	float theta = (atan(norm.y, norm.x) + PI) / (2.0 * PI);
	float r = min(1.0, length(norm));	

	float maxIterations = textureSize(u_Texture, 0).y;

	float strength = 1.0;
	for(float f = 0.0; f < r; f += 1.0 / textureSize(u_Texture, 0).y)
	{
		vec2 coord = vec2(theta, f);
		strength -= mix(1.0 / maxIterations, 5.0 / maxIterations, 1.0 - texture(u_Texture, coord).r);
	}

	out_FragColor = vec4(u_Color * pow(max(0.0, strength), 0.75), 1.0);
}