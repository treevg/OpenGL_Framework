#version 430

out vec4 fragColor;

uniform sampler2D mipmapTexture;
uniform int level;

int s = 2;

void main() {
    fragColor = vec4(0,0,0,0);
    
    if (level == 0) {
        ivec2 newCoord = ivec2(gl_FragCoord.xy);
        fragColor = vec4(texelFetch(mipmapTexture, newCoord, 0).xyz,1);
    } else {
        float count = 0;
        ivec2 coord = ivec2(gl_FragCoord.xy) * 2;

        for (int x = -s; x <= s; x++) {
            for (int y = -s; y <= s; y++) {
                vec4 tempColor = texelFetch(mipmapTexture, coord + ivec2(x, y), level - 1);
                if (tempColor.w > 0 && tempColor.x > 0.0000001 && !isnan(tempColor.x)) {
                    count += 1;
                    fragColor += tempColor;
                }
            }
        }

        if (count <= 3) {
            fragColor = vec4(0,0,0,0);
            return;
        } else {
            fragColor = vec4((fragColor / count).xyz, 1);
        }
    }
}