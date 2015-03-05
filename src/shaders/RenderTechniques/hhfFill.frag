#version 430

out vec4 fragColor;

uniform sampler2D mipmapTexture;

uniform vec2 resolution;
uniform int level;

ivec2 newRes = ivec2(resolution * pow(0.5, level+1));

void main() {
    fragColor = texelFetch(mipmapTexture, ivec2(gl_FragCoord.xy  - 0.5), level);

    if (fragColor.w == 0 || fragColor.x == 0 || isnan(fragColor.w)) {
        int count = 0;
        int s = 1;

        for (int x = -s; x <= s; x++) {
            for (int y = -s; y <= s; y++) {
                ivec2 newCoord = clamp(ivec2(gl_FragCoord.xy * 0.5) + ivec2(x, y), ivec2(0,0), newRes - 1);
                vec4 c = texelFetch(mipmapTexture, newCoord, level + 1);
                count ++;
                fragColor += c;
            }
        }

        if (count == 1) {
            fragColor = vec4(1,0,0,0);
        } else {
            fragColor /= count;
        }
    }
}