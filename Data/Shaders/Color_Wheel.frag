in vec2 v_TexCoord;
out vec4 out_FragColor;

#define PI 3.14159265358979

uniform float u_Value;

vec3 HSLtoRGB(in float h, in float s, in float l)
{
	vec3 rgb = clamp(abs(mod(h+vec3(0.0,4.0,2.0),6.0)-3.0)-1.0, 0.0, 1.0);
    rgb = smoothstep(0.0, 1.0, rgb);
	return l + s * (rgb-0.5)*(1.0-abs(2.0*l-1.0));
}

float bell(float x, float a, float c)
{
    return 0.5 + a * 0.5 * exp(-(x * x) / (2.0 * c * c));
}

void main()
{
    vec2 coord = v_TexCoord * 2.0 - 1.0;
	float dist = length(coord);
    float h = 6.0 * (atan(coord.y, coord.x) + PI) / (2.0 * PI);
    float s = bell(dist, u_Value, 0.5);
	out_FragColor = vec4(HSLtoRGB(clamp(h, 0.0, 360.0), 1.0, clamp(s, 0.0, 1.0)), 1.0) * float(dist <= 1.0);
}