varying vec2 v_texCoord;
varying vec2 v_chunkCoord;

uniform sampler2D u_texture;
uniform sampler2D u_shadowTexture;
uniform vec2 u_chunkOffset;

void main()
{
	vec4 a = texture2D(u_texture, v_texCoord);
	vec4 b = texture2D(u_shadowTexture, u_chunkOffset + v_chunkCoord);
	gl_FragColor.rgb = a.rgb*b.a;
	gl_FragColor.a = a.a;
	gl_FragColor = b;
}