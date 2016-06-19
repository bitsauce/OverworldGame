in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform sampler2D u_LightMap;
uniform vec2 u_LightTexCoord;
uniform vec2 u_Radius;
uniform vec3 u_Color;
uniform int u_Iterations;

void main()
{
	float sum = 1.0;
	for(float i = 1.0; i > 0.0; i -= 1.0 / float(u_Iterations))
	{
		sum *= 1U - min(1U, uint(texture(u_LightMap, u_LightTexCoord + (mix(v_TexCoord, vec2(0.0), i) * 2.0 - vec2(1.0)) * u_Radius)[1] * 255.5));
	}
	out_FragColor = vec4(u_Color * sum, 1.0);
}