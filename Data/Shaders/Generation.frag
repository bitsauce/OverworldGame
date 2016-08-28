in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform vec2 u_Position;
uniform vec2 u_Resolution;
uniform uint u_Seed;
uniform float u_Time;
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

#define BLOCK_EMPTY 0U
#define BLOCK_GRASS 1U
#define BLOCK_DIRT 2U
#define BLOCK_DIRT_BACK 4U
#define BLOCK_STONE 3U
//#define BLOCK_STONE_BACK 5U

void main()
{
	vec2 blockPos = floor(v_TexCoord * u_Resolution + u_Position);
	float f = u_Resolution.x / 720.0; // TODO: f can be integrated into u_Resolution (by u_Resolution /= f)

	// Get noise value
	float height = /*baseHeight +*/ mix(HEIGHT_MAX, HEIGHT_MIN - u_CliffingDelta, fractalNoise1D(4, 5.0 * f * blockPos.x / u_Resolution.x));

	// Get distance to ground
	float distanceToGround = height - blockPos.y;

	// Create cliffs/overhangs
	distanceToGround += fractalNoise2D(4, 20.0 * f * blockPos / u_Resolution) * u_CliffingDelta;

	// Get block id
	uvec3 blockIDs = uvec3(BLOCK_EMPTY);
	if(distanceToGround < -20)
	{
		blockIDs[1] = BLOCK_DIRT;//STONE;
	}
	else if(distanceToGround < 0)
	{
		blockIDs[1] = BLOCK_DIRT;
	}

	if(distanceToGround < -25)
	{
		blockIDs[0] = BLOCK_DIRT_BACK;//BLOCK_STONE_BACK;
	}
	else if(distanceToGround < -5)
	{
		blockIDs[0] = BLOCK_DIRT_BACK;
	}
	else // DEBUG
	{
		//blockIDs[0] = BLOCK_DIRT_BACK;
	}


	float freq = fractalNoise2D(4, 0.1 * f * blockPos / u_Resolution);
	if(abs(fractalNoise2D(4, 10.0 * f * blockPos / u_Resolution) - 0.5) < 0.1 * freq * clamp(-distanceToGround / 70.0, 0.0, 1.0)) blockIDs[1] = BLOCK_EMPTY;

	/*if(blockPos.y > (sin(blockPos.x * 0.05) + 1.0) * 10.0)
	{
		blockIDs[0] = BLOCK_DIRT_BACK;
		blockIDs[1] = BLOCK_DIRT;
	}
	else
	{
		blockIDs[0] = BLOCK_DIRT_BACK;
		blockIDs[1] = BLOCK_EMPTY;
	}*/

	// Set block color
	out_FragColor = vec4(float(blockIDs[0]) / 255.5, float(blockIDs[1]) / 255.5, float(blockIDs[2]) / 255.5, 0.0);
}