varying vec2 v_texCoord;

uniform sampler2D u_lightMap;
uniform vec2 u_lightTexCoord;
uniform vec2 u_radius;
uniform int u_iterations;

void main()
{
	vec2 dir = (vec2(0.5) - v_texCoord)/float(u_iterations);

	float sum = 1.0;
	for(float i = 0.0; i < float(u_iterations); i += 1.0)
	{
		float a = texture2D(u_lightMap, u_lightTexCoord + ((v_texCoord + dir * i) * 2.0 - vec2(1.0)) * u_radius).r;
		if(a == 0.0)
		sum -= 1.0-texture2D(u_lightMap, u_lightTexCoord + ((v_texCoord + dir * i) * 2.0 - vec2(1.0)) * u_radius).r;
	}
	//sum /= float(u_iterations);

	gl_FragColor = vec4(vec3(sum), 1.0);
	
	//gl_FragColor = texture2D(u_lightMap, u_lightTexCoord + (v_texCoord * 2.0 - vec2(1.0)) * u_radius);
	//gl_FragColor.rgb = vec3(1.0) - gl_FragColor.rgb;
}