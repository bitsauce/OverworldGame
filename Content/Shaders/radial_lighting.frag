varying vec2 v_texCoord;

uniform sampler2D u_lightMap;
uniform vec2 u_lightTexCoord;
uniform vec2 u_radius;
uniform vec3 u_color;
uniform int u_iterations;

void main()
{
	float sum = 1.0;
	for(float i = 1.0; i > 0.0; i -= 1.0/float(u_iterations))
	{
		sum *= texture2D(u_lightMap, u_lightTexCoord + (mix(v_texCoord, vec2(0.5), i) * 2.0 - vec2(1.0)) * u_radius).a;
	}
	gl_FragColor = vec4(u_color * vec3(sum), 1.0);
}


/*void main()
{
	vec2 dir = (vec2(0.5) - v_texCoord)/float(u_iterations);
	float sum = 1.0;
	for(float i = 0.0; i < float(u_iterations); i += 1.0)
	{
		sum -= texture2D(u_lightMap, u_lightTexCoord + ((v_texCoord + dir * i) * 2.0 - vec2(1.0)) * u_radius).g;
	}
	gl_FragColor = vec4(vec3(sum), 1.0);
}*/