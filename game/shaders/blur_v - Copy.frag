varying vec2 v_texCoord;

uniform sampler2D u_texture;
uniform float u_exponent;
uniform int u_radius;
uniform int u_height;

void main()
{
	gl_FragColor = vec4(0.0);
	for(float i = -float(u_radius); i <= float(u_radius); ++i)
	{
		gl_FragColor.a += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y + i/float(u_height))).a;
	}
	gl_FragColor.a = pow(abs(gl_FragColor.a / (float(u_radius)*2.0 + 1.0)), u_exponent);
}