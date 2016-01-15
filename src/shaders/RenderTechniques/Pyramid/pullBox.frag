#version 430

out vec4 fragColor;

uniform sampler2D inputTex;
uniform sampler2D pyramid_fragColor;
uniform int pyramid_level;
uniform int radius = 0;

void main() {
    if (pyramid_level == 0) {
        fragColor = texelFetch(inputTex, ivec2(gl_FragCoord.xy), 0);
        return;
    }

    ivec2 coord = ivec2(gl_FragCoord.xy * 2 - 1);
    
    fragColor = vec4(0,0,0,0);

    int count = 0;
    for (int i = -radius; i < 2 + radius; i++) {
        for (int j = -radius; j < 2 + radius; j++) {
            count++;
            fragColor += texelFetch(pyramid_fragColor, ivec2(i, j) + coord, pyramid_level - 1);
        }
    }

    fragColor /= count;
}