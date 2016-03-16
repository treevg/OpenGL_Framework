#version 430

out vec4 fragColor;

uniform sampler2D m_hhfTexture;
//uniform sampler2D expandedTexture;

uniform vec2 m_hhfResolution;

uniform int m_hhfMipmapLevel;

ivec2 lowerRes = ivec2(m_hhfResolution * pow(0.5, m_hhfMipmapLevel +1));

int s = 2;

void main()
{
	
	// vec4 currentColor = texelFetch(m_hhfTexture, ivec2(gl_FragCoord.xy-0.5), m_hhfMipmapLevel);
	vec4 currentColor = texelFetch(m_hhfTexture, ivec2(gl_FragCoord.xy), m_hhfMipmapLevel);

	if (currentColor.a == 0)
	{
		fragColor = currentColor;
	}
	
	else
	{
		int count = 0;
	   
		vec4 tempColor = vec4(0,0,0,0);
		
		for (int x = -s; x <= s; x++)
		{
			for (int y = -s; y <= s; y++)
			{	
				ivec2 newCoord = clamp(ivec2(gl_FragCoord.xy*0.5) + ivec2(x, y), ivec2(0,0), lowerRes - 1);
				vec4 c = texelFetch(m_hhfTexture, newCoord, m_hhfMipmapLevel + 1);
				count ++;
				tempColor.xyz += c.xyz;
			}
		}

		if (count > 0)
		{
			fragColor = vec4((tempColor.xyz)/count,1);	
		}
		
		else
		{
			fragColor = vec4(0.0,0.0,0.0,1);
		}
		
	}
}