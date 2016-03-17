#version 430

out vec4 fragColor;
uniform sampler2D m_hhfTexture;

uniform vec2 m_hhfResolution;
uniform int m_hhfMipmapLevel;

ivec2 lowerRes = ivec2(m_hhfResolution * pow(0.5, m_hhfMipmapLevel +1));
const int r = 2;

void main()
{
	vec4 centerColor = texelFetch(m_hhfTexture, ivec2(gl_FragCoord.xy), m_hhfMipmapLevel);
    if (centerColor.a == 0)
	{
		int count = 0;
        for (int x = -r; x <= r; x++)
		{
            for (int y = -r; y <= r; y++)
			{
				vec4 surroundingColor = texelFetch(m_hhfTexture, ivec2(gl_FragCoord.xy)+ivec2(x,y), m_hhfMipmapLevel);
				bool isPC = (surroundingColor.a != 0);
				if( isPC )
				{
					++count;
				}
			}
		}
		if( count >= 15)
		{
			//ivec2 mipMapCoord = clamp(ivec2(gl_FragCoord.xy*0.5) + ivec2(x, y), ivec2(0,0), lowerRes - 1);
			ivec2 mipMapCoord = clamp(ivec2(gl_FragCoord.xy*0.5), ivec2(0,0), lowerRes - 1);
			vec4 mipMapColor = texelFetch(m_hhfTexture, mipMapCoord, m_hhfMipmapLevel + 1);
			fragColor = mipMapColor;
		}
		else
		{
			fragColor = centerColor;
		}
    }
	else
	{
		fragColor = centerColor;
	}
}

 //   int a[3];
 //   int b[] = a;
 //   b = int[3]{2, 3, 4};
	//float floatArray[4] = {1.1, 2.2, 3.3, 4.4};
 //   float floatArray2[4] = float[4]{1.1, 2.2, 3.3, 4.4};
 //   float floatArray3[4] = float[]{1.1, 2.2, 3.3, 4.4};