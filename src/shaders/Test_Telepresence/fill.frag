#version 430

out vec4 fragColor;

uniform sampler2D m_hhfTexture;
//uniform sampler2D expandedTexture;

uniform vec2 m_hhfResolution;

uniform int m_hhfMipmapLevel;

ivec2 newRes = ivec2(m_hhfResolution * pow(0.5, m_hhfMipmapLevel));

int s = 2;
void main() {
    // fragColor = texelFetch(m_hhfTexture, ivec2(gl_FragCoord.xy), m_hhfMipmapLevel+1);
	// int count = 0;

    // if (fragColor.w == 0 || (fragColor.x == 1 && fragColor.y == 1 && fragColor.z == 1) || isnan(fragColor.w)) {
     // count++;   
	
        // for (int x = -s; x <= s; x++) {
            // for (int y = -s; y <= s; y++) {
                // ivec2 newCoord = clamp(ivec2(gl_FragCoord.xy + ivec2(x/m_hhfResolution.x, y/m_hhfResolution.y)), ivec2(0,0), newRes - 1);
                // vec4 c = texelFetch(m_hhfTexture, newCoord, m_hhfMipmapLevel + 1);
                // count ++;
                // fragColor += c;
            // }
        // }
    // }
//	fragColor = texelFetch(m_hhfTexture, ivec2(gl_FragCoord.xy),m_hhfMipmapLevel+1);
	   // if (count == 0) {
            // fragColor = vec4(1,0,0,1);
        // } 
	//fragColor = vec4(texelFetch(m_hhfTexture,  ivec2(gl_FragCoord.xy  - 0.5), m_hhfMipmapLevel).xyz,1);
	//ivec2 newCoord = ivec2( gl_FragCoord.xy-0.5);
	//fragColor = texelFetch(m_hhfTexture, newCoord, m_hhfMipmapLevel+1);
	
    fragColor = texelFetch(m_hhfTexture, ivec2(gl_FragCoord.xy-0.5), m_hhfMipmapLevel);
	//fragColor = vec4(1,1,1,1);
   // if (fragColor.w < 0.00001 || (fragColor.x >= 0.9 && fragColor.y >= 0.9 && fragColor.z >= 0.9) || isnan(fragColor.w)) {
	//	if (fragColor.w < 0.00001 || (fragColor.x == 1 || fragColor.y == 1 || fragColor.z == 1) || isnan(fragColor.w)) {
        int count = 0;
       
		vec4 tempColor = vec4(0,0,0,0);
		
        for (int x = -s; x <= s; x++) {
            for (int y = -s; y <= s; y++) {
				
                ivec2 newCoord = clamp(ivec2(gl_FragCoord.xy*0.5) + ivec2(x, y), ivec2(0,0), newRes - 1);
                vec4 c = texelFetch(m_hhfTexture, newCoord, m_hhfMipmapLevel + 1);
				if(c.x  == 1.0 || c.y == 1.0 || c.z == 1.0){
					count ++;
					tempColor += c;
				}
            }
        }

        if (count > 0) {
			fragColor = tempColor/count;
			 //fragColor = vec4(1,0,0,1);
		}
        // } else {
			 // fragColor = vec4(0,1,0,1);
          //  fragColor /= count;
        // }
   // }
	
}