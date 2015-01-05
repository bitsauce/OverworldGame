varying vec2 v_texCoord;

uniform sampler2D u_texture;
uniform float u_offsetY;
uniform float u_direction;
uniform float u_width;
uniform float u_height;

void main()
{
	float factor = v_texCoord.y - u_offsetY;
	if(factor < 0.1) factor = 0.0;
	
	float sum = 0.0;
	for(int i = -5; i <= 5; i++)
	{
		sum += texture2D(u_texture, v_texCoord + vec2(float(i) / u_width, float(i) / u_height) * vec2(cos(u_direction), sin(u_direction)) * factor).a;
	}
	sum /= 10.0;
	
	gl_FragColor = vec4(vec3(0.0), sum);
	
	//gl_FragColor = vec4(vec3(0.0), min(texture2D(u_texture, v_texCoord).a, min(texture2D(u_texture, v_texCoord + vec2(1.0/u_width, 0.0)).a, min(texture2D(u_texture, v_texCoord - vec2(1.0/u_width, 0.0)).a, min(texture2D(u_texture, v_texCoord + vec2(0.0, 1.0/u_height)).a, texture2D(u_texture, v_texCoord - vec2(0.0, 1.0/u_height)).a)))));
}