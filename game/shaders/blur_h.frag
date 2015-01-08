varying vec2 v_texCoord;

uniform sampler2D u_texture;
uniform int u_width;

void main()
{
	float blur = 0.0;
	blur += texture2D(u_texture, vec2(v_texCoord.x - 4.0*(1.0/float(u_width)), v_texCoord.y)).r * 0.0162162162;
	blur += texture2D(u_texture, vec2(v_texCoord.x - 3.0*(1.0/float(u_width)), v_texCoord.y)).r * 0.0540540541;
	blur += texture2D(u_texture, vec2(v_texCoord.x - 2.0*(1.0/float(u_width)), v_texCoord.y)).r * 0.1216216216;
	blur += texture2D(u_texture, vec2(v_texCoord.x - 1.0*(1.0/float(u_width)), v_texCoord.y)).r * 0.1945945946;
	blur += texture2D(u_texture, vec2(v_texCoord.x,                            v_texCoord.y)).r * 0.2270270270;
	blur += texture2D(u_texture, vec2(v_texCoord.x + 1.0*(1.0/float(u_width)), v_texCoord.y)).r * 0.1945945946;
	blur += texture2D(u_texture, vec2(v_texCoord.x + 2.0*(1.0/float(u_width)), v_texCoord.y)).r * 0.1216216216;
	blur += texture2D(u_texture, vec2(v_texCoord.x + 3.0*(1.0/float(u_width)), v_texCoord.y)).r * 0.0540540541;
	blur += texture2D(u_texture, vec2(v_texCoord.x + 4.0*(1.0/float(u_width)), v_texCoord.y)).r * 0.0162162162;
	gl_FragColor = vec4(vec3(blur), 1.0);
}