in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform sampler2D u_Texture;
uniform float u_Exponent;
uniform int u_Height;

void main()
{
	vec3 blur = vec3(0.0);
	blur += texture(u_Texture, vec2(v_TexCoord.x, v_TexCoord.y - 4.0*(1.0/float(u_Height)))).rgb * 0.0162162162;
	blur += texture(u_Texture, vec2(v_TexCoord.x, v_TexCoord.y - 3.0*(1.0/float(u_Height)))).rgb * 0.0540540541;
	blur += texture(u_Texture, vec2(v_TexCoord.x, v_TexCoord.y - 2.0*(1.0/float(u_Height)))).rgb * 0.1216216216;
	blur += texture(u_Texture, vec2(v_TexCoord.x, v_TexCoord.y - 1.0*(1.0/float(u_Height)))).rgb * 0.1945945946;
	blur += texture(u_Texture, vec2(v_TexCoord.x, v_TexCoord.y)).rgb                             * 0.2270270270;
	blur += texture(u_Texture, vec2(v_TexCoord.x, v_TexCoord.y + 1.0*(1.0/float(u_Height)))).rgb * 0.1945945946;
	blur += texture(u_Texture, vec2(v_TexCoord.x, v_TexCoord.y + 2.0*(1.0/float(u_Height)))).rgb * 0.1216216216;
	blur += texture(u_Texture, vec2(v_TexCoord.x, v_TexCoord.y + 3.0*(1.0/float(u_Height)))).rgb * 0.0540540541;
	blur += texture(u_Texture, vec2(v_TexCoord.x, v_TexCoord.y + 4.0*(1.0/float(u_Height)))).rgb * 0.0162162162;
	blur *= 2.0;
	out_FragColor = vec4(blur, 1.0);
}