in vec2 v_TexCoord;
in vec2 v_BlockPos;
in float v_BlockEntityID;

out vec4 out_FragColor;

uniform sampler2D u_TextureAtlas;
uniform sampler2D u_TimeOffsets;
uniform sampler2D u_BlockEntityData;
uniform float u_Time;

void main()
{
	vec2 dataTexel = 1.0 / textureSize(u_BlockEntityData, 0);
	vec2 atlasTexel = 1.0 / textureSize(u_TextureAtlas, 0);

	float offset = floor(texture(u_TimeOffsets, v_BlockPos).r * 255.5);

	// data[0]: Width (px)
	// data[1]: Height (px)
	// data[2]: Frame count
	// data[3]: Animation speed
	vec4 data = floor(texture(u_BlockEntityData, (vec2(v_BlockEntityID, 1.0) + 0.5) * dataTexel) * 255.5);
	uvec4 dataUV = uvec4(texture(u_BlockEntityData, (vec2(v_BlockEntityID, 0.0) + 0.5) * dataTexel) * 255.5);
	
	vec2 atlasUV = vec2(float(dataUV[0] | (dataUV[1] << 8U)), float(dataUV[2] | (dataUV[3] << 8U)));

	float frameIndex = floor(mod(u_Time * data[3] * 1.0 / 8.5 + offset, data[2]));

	out_FragColor = texture(u_TextureAtlas, ((atlasUV + v_TexCoord * data.xy) * atlasTexel) + vec2(frameIndex * data[0] * atlasTexel.x, 0.0));
}