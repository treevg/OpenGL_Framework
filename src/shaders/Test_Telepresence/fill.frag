#version 430

out vec4 fragColor;

uniform sampler2D m_hhfTexture;
//uniform sampler2D expandedTexture;

//uniform vec2 m_hhfResolution;

uniform int m_hhfMipmapLevel;

//ivec2 newRes = ivec2(m_hhfResolution * pow(0.5, m_hhfMipmapLevel+1));

void main() {
    //fragColor = texelFetch(m_hhfTexture, ivec2(gl_FragCoord.xy  - 0.5), m_hhfMipmapLevel);

/*     if (fragColor.w == 0 || fragColor.x == 0 || isnan(fragColor.w)) {
        int count = 0;
        int s = 1;

        for (int x = -s; x <= s; x++) {
            for (int y = -s; y <= s; y++) {
                ivec2 newCoord = clamp(ivec2(gl_FragCoord.xy * 0.5) + ivec2(x, y), ivec2(0,0), newRes - 1);
                vec4 c = texelFetch(m_hhfTexture, newCoord, m_hhfMipmapLevel + 1);
                count ++;
                fragColor += c;
            }
        }

        if (count == 0) {
            fragColor = vec4(1,0,0,1);
        } else {
            fragColor /= count;
        }
    }  */
	//fragColor = vec4(texelFetch(m_hhfTexture,  ivec2(gl_FragCoord.xy  - 0.5), m_hhfMipmapLevel).xyz,1);
	ivec2 newCoord = ivec2( gl_FragCoord.xy);
	fragColor = texelFetch(m_hhfTexture, newCoord, m_hhfMipmapLevel);
}