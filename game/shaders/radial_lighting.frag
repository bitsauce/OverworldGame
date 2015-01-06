varying vec2 v_texCoord;

uniform sampler2D u_texture;
uniform int u_iterations;
uniform vec2 u_position;
uniform float u_radius;

#define PI 3.1415926535897932384626433832795

void main()
{
	float sum = 0.0;
	for(float i = 0; i < float(u_iterations); i += 1.0)
	{
		sum += texture2D(u_texture, v_texCoord + vec2());
	}

	gl_FragColor = vec4();
}