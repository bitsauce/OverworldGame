in vec2 in_position;
in vec4 in_vertexColor;
in vec2 in_texCoord;

out vec2 v_texCoord;

uniform mat4 u_modelViewProj;

void main()
{
	gl_Position = vec4(in_position, 0.0, 1.0) * u_modelViewProj;
	v_texCoord = in_texCoord;
}