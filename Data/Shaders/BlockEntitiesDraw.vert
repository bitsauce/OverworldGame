in vec2 in_Position;
in vec4 in_VertexColor;
in vec2 in_TexCoord;

out vec2 v_TexCoord;
out vec2 v_BlockPos;
out float v_BlockEntityID;

uniform mat4 u_ModelViewProj;

void main()
{
	gl_Position = vec4(in_Position, 0.0, 1.0) * u_ModelViewProj;
	v_TexCoord = in_TexCoord;
	v_BlockPos = floor(in_VertexColor.rg * 255.5) / 32.0;
	v_BlockEntityID = floor(in_VertexColor.b * 255.5);
}