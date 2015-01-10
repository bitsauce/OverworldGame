varying vec2 v_texCoord;

uniform sampler2D u_texture;
uniform float u_exponent;
uniform int u_height;

void main()
{
	vec3 blur = vec3(0.0);
	blur += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y - 4.0*(1.0/float(u_height)))).rgb * 0.0162162162;
	blur += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y - 3.0*(1.0/float(u_height)))).rgb * 0.0540540541;
	blur += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y - 2.0*(1.0/float(u_height)))).rgb * 0.1216216216;
	blur += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y - 1.0*(1.0/float(u_height)))).rgb * 0.1945945946;
	blur += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y)).rgb                             * 0.2270270270;
	blur += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y + 1.0*(1.0/float(u_height)))).rgb * 0.1945945946;
	blur += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y + 2.0*(1.0/float(u_height)))).rgb * 0.1216216216;
	blur += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y + 3.0*(1.0/float(u_height)))).rgb * 0.0540540541;
	blur += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y + 4.0*(1.0/float(u_height)))).rgb * 0.0162162162;
	gl_FragColor = vec4(vec3(1.0) - pow(vec3(1.0) - blur, vec3(u_exponent)), 1.0);
}