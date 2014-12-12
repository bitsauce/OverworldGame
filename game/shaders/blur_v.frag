varying vec2 v_texCoord;

uniform sampler2D u_texture;
uniform float u_exponent;
uniform int u_radius;
uniform int u_height;

void main()
{
	gl_FragColor = vec4(0.0);
	gl_FragColor += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y - 4.0*(1.0/u_height))) * 0.0162162162;
	gl_FragColor += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y - 3.0*(1.0/u_height))) * 0.0540540541;
	gl_FragColor += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y - 2.0*(1.0/u_height))) * 0.1216216216;
	gl_FragColor += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y - 1.0*(1.0/u_height))) * 0.1945945946;
	gl_FragColor += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y))                      * 0.2270270270;
	gl_FragColor += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y + 1.0*(1.0/u_height))) * 0.1945945946;
	gl_FragColor += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y + 2.0*(1.0/u_height))) * 0.1216216216;
	gl_FragColor += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y + 3.0*(1.0/u_height))) * 0.0540540541;
	gl_FragColor += texture2D(u_texture, vec2(v_texCoord.x, v_texCoord.y + 4.0*(1.0/u_height))) * 0.0162162162;
	gl_FragColor.a = pow(gl_FragColor.a, u_exponent);
}