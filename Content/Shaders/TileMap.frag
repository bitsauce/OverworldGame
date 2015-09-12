in vec2 v_texCoord;

out vec4 out_fragColor;

uniform usampler2D u_tileMap;

void main()
{
	//out_fragColor = vec4(vec3(float(texture(u_tileMap, v_texCoord).r) / 255.0), 1.0);
	out_fragColor = vec4(float(texture(u_tileMap, v_texCoord)) / 255.0);
}