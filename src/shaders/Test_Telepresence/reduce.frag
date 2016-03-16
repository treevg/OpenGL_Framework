#version 430

out vec4 fragColor;
 in vec4 passPosition;
 
uniform sampler2D m_pcOutputTex;

uniform int m_hhfMipmapLevel;
uniform int m_hhfResolution;

ivec2 m_hhfNewRes = ivec2(m_hhfResolution * pow(0.5, m_hhfMipmapLevel+1));

// window radius (short r) (1=3x3 ,2 = 5x5, 3= 7x7...)
// uniform int windowradius;
int r = 2;
int countmax = (r*2 +1) * (r*2 +1);
int count;


vec4 LookUpSurroundingColors( vec4 currentPixelColor )
{
	vec4 color = vec4(0.0);
	
	// texture coordinate of mipmap level above
	ivec2 ulcoord = ivec2(gl_FragCoord.xy)*2;
	
	// iterate over upper level picture
	for(int x = -r; x <= r; x++)
	{
		for(int y = -r; y <= r; y++)
		{
			// get color from neighbor pixel
			vec4 tempColor =  texelFetch(m_pcOutputTex, ulcoord+ivec2(x,y) , m_hhfMipmapLevel-1);
			
			// check if pixel is from background
			if(tempColor.a == 0)
			{
				count += 1;
				// add color to tempcolor, will be divided / number
				color += 1.2 * tempColor;
			}
			else
			{
				color += 0.8 * tempColor;
			}
		}
	}
	
	// everything is background
	if (count == countmax)
	{
		color = currentPixelColor;
	}
	
	// some pixels are background
	else if ((count > 0) && (count < countmax))
	{
		color /= countmax;
	}
	
	// all pixels are pointcloud
	else
	{
		color = vec4(1,0,0,1);
	}
	
	return color;
}

void main()
{
	
	// count non-zero pixel
	count = 0;
	
	vec4 currentPixelColor = texelFetch(m_pcOutputTex, ivec2(gl_FragCoord.xy), 0);
	
	// if top level
	if (m_hhfMipmapLevel == 0)
	{
		// use texture color
		fragColor = currentPixelColor;
	}
	
	// if not top level
	else
	{		
		fragColor = LookUpSurroundingColors( currentPixelColor);
	}
	
}
