#version 430

out vec4 fragColor;

uniform sampler2D tex;
uniform sampler2D pyramid_fragColor;
uniform int pyramid_level;

void main() {
    if (pyramid_level == 0) {
        fragColor = texelFetch(tex, ivec2(gl_FragCoord.xy), 0);
    } else {     
        fragColor = vec4(0,0,0,0);
        int count = 0;
        for (int x = 0; x <= 1; x++) {
            for (int y = 0; y <= 1; y++) {
                vec4 color = texelFetch(pyramid_fragColor, ivec2(x, y) + ivec2(gl_FragCoord.xy) * 2, pyramid_level - 1);
                if (color.a > 0) {
                    count++;
                    fragColor += color;
                }
            }
        }
        if (count > 0) {
            fragColor = vec4(fragColor.rgb / count, 1);
        }
    }
}