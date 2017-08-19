in vec2 v_TexCoord;
in vec2 v_BlockPos;
in float v_BlockEntityID;

out vec4 out_FragColor;

uniform sampler2D u_TextureAtlas;
uniform sampler2D u_OffsetUVs;
uniform sampler2D u_BlockEntityData;
uniform float u_Time;

void main()
{
	vec2 dataTexel = 1.0 / textureSize(u_BlockEntityData, 0);
	vec2 atlasTexel = 1.0 / textureSize(u_TextureAtlas, 0);

	// data[0]: Width (px)
	// data[1]: Height (px)
	// data[2]: Frame columns
	// data[3]: Frame rows
	vec4 data = floor(texture(u_BlockEntityData, (vec2(v_BlockEntityID, 1.0) + 0.5) * dataTexel) * 255.5);
	uvec4 dataUV = uvec4(texture(u_BlockEntityData, (vec2(v_BlockEntityID, 0.0) + 0.5) * dataTexel) * 255.5);
	uvec4 offsetUV = uvec4(texture(u_OffsetUVs, v_BlockPos) * 255.5);
	vec2 atlasUV = vec2(float(dataUV[0] | (dataUV[1] << 8U)), float(dataUV[2] | (dataUV[3] << 8U))) +
	               vec2(float(offsetUV[0] | (offsetUV[1] << 8U)), float(offsetUV[2] | (offsetUV[3] << 8U)));
	
	float frameIndex = mod(u_Time, data[2] * data[3]);
	float frameX = floor(mod(frameIndex, data[2]));
	float frameY = floor(frameIndex / data[3]);

	out_FragColor = texture(u_TextureAtlas, ((atlasUV + v_TexCoord * data.xy) * atlasTexel) + vec2(frameX * data[0] * atlasTexel.x, frameY * data[1] * atlasTexel.y));
}