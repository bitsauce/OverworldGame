in vec2 v_TexCoord;
out vec4 out_FragColor;

uniform sampler2D u_Texture;

void main()
{
	vec2 northTexCoord = v_TexCoord - vec2(0.0, 1.0 / textureSize(u_Texture, 0).y);
	vec2 southTexCoord = v_TexCoord + vec2(0.0, 1.0 / textureSize(u_Texture, 0).y);
	vec2 westTexCoord = v_TexCoord - vec2(1.0 / textureSize(u_Texture, 0).x, 0.0);
	vec2 eastTexCoord = v_TexCoord + vec2(1.0 / textureSize(u_Texture, 0).x, 0.0);

	
	// Set top row to 0
	if(northTexCoord.y < 0.0)
	{
		out_FragColor = vec4(0.0);
	}
	else
	{
		// Get this block's water levels
		vec4 center = texture(u_Texture, v_TexCoord);

		// If this block is on the ground
		float south = texture(u_Texture, southTexCoord).b;
		if(southTexCoord.y > 1.0 || south >= 0.25)
		{
			out_FragColor = texture(u_Texture, northTexCoord) + texture(u_Texture, westTexCoord) * 0.5 + texture(u_Texture, eastTexCoord) * 0.5;
		}
		else
		{
			// Take from the block above
			out_FragColor = texture(u_Texture, northTexCoord);
		}
	}

	out_FragColor.a = 1.0;
}