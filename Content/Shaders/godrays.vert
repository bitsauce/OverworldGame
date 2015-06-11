// 
varying vec2 vTexcoord;

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;
	vTexcoord = gl_MultiTexCoord0.xy;
}