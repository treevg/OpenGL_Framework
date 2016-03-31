#version 430

out vec4 fragColor;
 in vec4 passPosition;
 
uniform sampler2D m_pcOutputTex;

uniform int m_hhfMipmapLevel;
//uniform vec2 m_hhfResolution;

//ivec2 m_hhfNewRes = ivec2(m_hhfResolution * pow(0.5, m_hhfMipmapLevel+1));

const int m_r = 2;
const int m_side = 5;
const int m_regionSize = 25;
vec4 m_regionArray[m_regionSize];
const float m_maskArray[m_regionSize] = 
{
	1.0,  4.0,  7.0,  4.0, 1.0, 
	4.0, 16.0, 26.0, 16.0, 4.0, 
	7.0, 26.0, 41.0, 26.0, 7.0, 
	4.0, 16.0, 26.0, 16.0, 4.0, 
	1.0,  4.0,  7.0,  4.0, 1.0
};

float accessMask(int x, int y)
{
	int index = y * m_side + x;
	return m_maskArray[index];
}

vec4 weighColor( float pcCount, vec4 pcColor, float bgCount, vec4 bgColor)
{
	if( pcCount > 0.0 )
	{
		return pcColor / pcCount;
	}
	return bgColor/bgCount;
	//return vec4(1.0, 1.0, 0.0, 0.0);
}

vec4 LookUpSurroundingColors()
{
	// texture coordinate of mipmap level above
	ivec2 ulcoord = ivec2(gl_FragCoord.xy)*2;
	
	vec4 pcColor = vec4(0.0);
	vec4 bgColor = vec4(0.0);
	float pcCount = 0.0;
	float bgCount = 0.0;

	// iterate over upper level picture
	for(int x = -m_r; x <= m_r; x++)
	{
		for(int y = -m_r; y <= m_r; y++)
		{
			// get color from neighbor pixel
			vec4 texelInfo =  texelFetch(m_pcOutputTex, ulcoord+ivec2(x,y) , m_hhfMipmapLevel-1);
			float currentWeight = texelInfo.a;
			bool isPC = (currentWeight != 0);
			
			//fillRegion(texelInfo, x + m_r, y + m_r);

			float gauss = accessMask( x + m_r, y + m_r);
			if(isPC)
			{
				pcCount += gauss;
				pcColor += texelInfo * gauss;
			}
			else
			{
				bgCount += gauss;
				bgColor += texelInfo * gauss;
			}
		}
	}
	
	return weighColor( pcCount, pcColor, bgCount, bgColor);
}

vec4 simpleReduce()
{
	// texture coordinate of mipmap level above
	ivec2 ulcoord = ivec2(gl_FragCoord.xy)*2;
	vec4 texelInfo =  texelFetch(m_pcOutputTex, ulcoord , m_hhfMipmapLevel-1);
	return texelInfo;
}

void main()
{
	fragColor = LookUpSurroundingColors();
	//fragColor = simpleReduce();
	
}