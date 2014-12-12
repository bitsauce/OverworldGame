varying vec2 v_texCoord;
varying vec2 v_chunkCoord;

void main()
{
	v_chunkCoord = gl_Vertex.xy/vec2(16.0*16.0, 16.0*16.0);
	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;
	v_texCoord = gl_MultiTexCoord0.xy;
}