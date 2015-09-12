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
	for(float i = 1.0; i > 0.0; i -= 1.0/float(u_Iterations))
	{
		sum *= texture(u_LightMap, u_LightTexCoord + (mix(v_TexCoord, vec2(0.5), i) * 2.0 - vec2(1.0)) * u_Radius).a;
	}
	out_FragColor = vec4(u_Color * vec3(sum), 1.0);
}


/*void main()
{
	vec2 dir = (vec2(0.5) - v_TexCoord)/float(u_Iterations);
	float sum = 1.0;
	for(float i = 0.0; i < float(u_Iterations); i += 1.0)
	{
		sum -= texture2D(u_LightMap, u_LightTexCoord + ((v_TexCoord + dir * i) * 2.0 - vec2(1.0)) * u_Radius).g;
	}
	out_FragColor = vec4(vec3(sum), 1.0);
}*/