in vec2 v_TexCoord;
in vec2 v_BlockPos;

out vec4 out_FragColor;

uniform sampler2D u_TextureAtlas;
uniform sampler2D u_TimeOffsetTexture;
uniform float u_Time;
//uniform sampler2D u_BlockEntityData;

void main()
{
	float offset = texture(u_TimeOffsetTexture, v_BlockPos).r;

	const float a = 16.0 / 2048.0;

	float frame = floor(mod(u_Time + offset, 1.0) * 3.0);

	vec4 color = texture(u_TextureAtlas, vec2(v_TexCoord.x + frame * a, v_TexCoord.y));

	out_FragColor = color;
}