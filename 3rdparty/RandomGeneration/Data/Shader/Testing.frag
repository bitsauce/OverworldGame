in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform float u_Time;
uniform vec2 u_Position;
uniform float u_Scale;

/*uint hash(uint seed)
{
	seed = (seed ^ 61U) ^ (seed >> 16U);
    seed *= 9U;
    seed = seed ^ (seed >> 4U);
    seed *= 0x27D4EB2DU;
    seed = seed ^ (seed >> 15U);
    return seed;
}*/

uint hash(uint x)
{
    x += (x << 10U);
    x ^= (x >>  6U);
    x += (x <<  3U);
    x ^= (x >> 11U);
    x += (x << 15U);
    return x;
}

float rand(vec2 n)
{
	return float(hash((uint(abs(n.x * 1280.241223)) << 16U) + uint(abs(n.y * 720.241223)))) * (1.0 / 4294967296.0);
}


float randMod289(vec2 pos)
{
	pos = mod(pos * 1.123123, vec2(8192.0, 2048.0)) - vec2(8192.0 * 0.5, 1024.0);
	pos.x = mod((pos.x * 34.0 + 1.0) * pos.x, 289.0) + pos.y;
	return mod((pos.x * 34.0 + 1.0) * pos.x, 289.0) / 289.0;
}


float randFract(vec2 co)
{
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
	float f = 0.0;
	for(int i = 0; i < 1000; i++)
	{
		f += rand(v_TexCoord + vec2(i * 0.000001));
	}

	f /= 1000.0;

	out_FragColor = vec4(
		vec3(f),
		1.0);
}