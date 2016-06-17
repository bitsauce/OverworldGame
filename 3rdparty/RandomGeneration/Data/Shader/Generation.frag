in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform bool u_ShowNoise;
uniform vec2 u_Position;
uniform vec2 u_Resolution;
uniform uint u_Seed;
uniform float u_Time;
uniform float u_Scale;
uniform float u_CliffingDelta;

int hash(int x)
{
    x += (x << 10U);
    x ^= (x >>  6U);
    x += (x <<  3U);
    x ^= (x >> 11U);
    x += (x << 15U);
    return x;
}

float rand(float x)
{
	return 0.5 * (float(hash(int(x * 1280.241223))) * (1.0 / 2147483647.0)) + 0.5;
}

float rand(vec2 pos)
{
	return 0.5 * (float(hash((int(pos.x * 1280.241223) << 16U) + int(pos.y * 720.241223))) * (1.0 / 2147483647.0)) + 0.5;
}

float fractalNoise1D(int octaves, float x)
{
	float value = 0.0;
	float amplitude = 0.5 / (1.0 - pow(2.0, -float(octaves)));
	for(int i = 0; i < octaves; i++)
	{
		x += float(u_Seed);
		float j = floor(x);
		float f = smoothstep(0.0, 1.0, fract(x));
		value += amplitude * mix(rand(j), rand(j + 1.0), f);
		x *= 2.0;
		amplitude *= 0.5;
	}
	return value;
}

float fractalNoise2D(int octaves, vec2 pos)
{
	float value = 0.0;
	float amplitude = 0.5 / (1.0 - pow(2.0, -float(octaves)));
	for(int i = 0; i < octaves; i++)
	{
		pos += vec2(float(u_Seed));
		vec2 j = floor(pos);
		vec2 f = smoothstep(0.0, 1.0, fract(pos));
		value += amplitude * mix(
								mix(rand(j + vec2(0.0, 0.0)),
									rand(j + vec2(1.0, 0.0)), f.x),
								mix(rand(j + vec2(0.0, 1.0)),
									rand(j + vec2(1.0, 1.0)), f.x),
								f.y);
		pos *= 2.0;
		amplitude *= 0.5;
	}
	return value;
}

#define HEIGHT_MIN 720
#define HEIGHT_MAX 0

// TODO: Need some automated system for storing the block IDs
/*enum BlockID
{
	BLOCK_EMPTY,

	BLOCK_GRASS,
	BLOCK_DIRT,
	BLOCK_DIRT_BACK,

	BLOCK_OAK_WOOD,
	BLOCK_OAK_LEAVES,

	BLOCK_STONE,
	BLOCK_STONE_BACK,

	BLOCK_ANIM_TEST,
	BLOCK_TORCH_TEST,

	//BLOCK_SAND,
	//BLOCK_MUD,
	//BLOCK_,

	BLOCK_COUNT
};*/

#define BLOCK_EMPTY 0U
#define BLOCK_GRASS 1U
#define BLOCK_DIRT 2U
#define BLOCK_DIRT_BACK 3U
#define BLOCK_STONE 6U
#define BLOCK_STONE_BACK 7U

void main()
{
	vec2 blockPos = floor(v_TexCoord * u_Resolution * u_Scale + u_Position);
	
	//float baseHeight = fractalNoise1D(4, 0.1 * blockPos.x / u_Resolution.x) * 10000.0;

	// Get noise value
	float height = /*baseHeight +*/ mix(HEIGHT_MAX, HEIGHT_MIN - u_CliffingDelta, fractalNoise1D(4, 5.0 * blockPos.x / u_Resolution.x));

	// Get distance to ground
	float distanceToGround = height - blockPos.y;

	// Create cliffs/overhangs
	distanceToGround += fractalNoise2D(4, 20.0 * blockPos / u_Resolution) * u_CliffingDelta;

	// Get blocks
	uvec4 blockIDs = uvec4(BLOCK_EMPTY);
	if(distanceToGround < -20)
	{
		blockIDs[1] = BLOCK_STONE;
	}
	else if(distanceToGround < 0)
	{
		blockIDs[1] = BLOCK_DIRT;
	}

	if(distanceToGround < -25)
	{
		blockIDs[0] = BLOCK_STONE_BACK;
	}
	else if(distanceToGround < -5)
	{
		blockIDs[0] = BLOCK_DIRT_BACK;
	}

	float freq = fractalNoise2D(4, 0.1 * blockPos / u_Resolution);
	if(abs(fractalNoise2D(4, 10.0 * blockPos / u_Resolution) - 0.5) < 0.1 * freq * clamp(-distanceToGround / 70.0, 0.0, 1.0)) blockIDs[1] = BLOCK_EMPTY;

	// Set block color
	if(u_ShowNoise)
	{
		out_FragColor = vec4(vec3(distanceToGround / (HEIGHT_MIN - HEIGHT_MAX)), 1.0);
	}
	else
	{
		out_FragColor = vec4(blockIDs) / 255.5;
	}

	//out_FragColor *= smoothstep(0.0, 5.0, abs(blockPos.y - HEIGHT_MAX));
	//out_FragColor *= smoothstep(0.0, 5.0, abs(blockPos.y - HEIGHT_MIN));
}