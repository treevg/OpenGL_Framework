#version 430

out vec4 fragColor;

uniform sampler2D pyramid_fragColor;
uniform int pyramid_level;

void main() {
    fragColor = vec4(0,0,0,0);

    vec4 c;

    c = texelFetch(pyramid_fragColor, ivec2(gl_FragCoord.xy) / 2, pyramid_level + 1);
    fragColor += c * 9.0/16.0;

    if (int(gl_FragCoord.x) % 2 == 0) {
        if (int(gl_FragCoord.y) % 2 == 0) {
            c = texelFetch(pyramid_fragColor, ivec2(-1,  0) + ivec2(gl_FragCoord.xy) / 2, pyramid_level + 1);
            fragColor += c * 3.0/16.0;
            c = texelFetch(pyramid_fragColor, ivec2( 0, -1) + ivec2(gl_FragCoord.xy) / 2, pyramid_level + 1);
            fragColor += c * 3.0/16.0;
            c = texelFetch(pyramid_fragColor, ivec2(-1, -1) + ivec2(gl_FragCoord.xy) / 2, pyramid_level + 1);
            fragColor += c * 1.0/16.0;
        } else {
            c = texelFetch(pyramid_fragColor, ivec2(-1,  0) + ivec2(gl_FragCoord.xy) / 2, pyramid_level + 1);
            fragColor += c * 3.0/16.0;
            c = texelFetch(pyramid_fragColor, ivec2( 0, +1) + ivec2(gl_FragCoord.xy) / 2, pyramid_level + 1);
            fragColor += c * 3.0/16.0;
            c = texelFetch(pyramid_fragColor, ivec2(-1, +1) + ivec2(gl_FragCoord.xy) / 2, pyramid_level + 1);
            fragColor += c * 1.0/16.0;
        }
    } else {
        if (int(gl_FragCoord.y) % 2 == 0) {
            c = texelFetch(pyramid_fragColor, ivec2(+1,  0) + ivec2(gl_FragCoord.xy) / 2, pyramid_level + 1);
            fragColor += c * 3.0/16.0;
            c = texelFetch(pyramid_fragColor, ivec2( 0, -1) + ivec2(gl_FragCoord.xy) / 2, pyramid_level + 1);
            fragColor += c * 3.0/16.0;
            c = texelFetch(pyramid_fragColor, ivec2(+1, -1) + ivec2(gl_FragCoord.xy) / 2, pyramid_level + 1);
            fragColor += c * 1.0/16.0;
        } else {
            c = texelFetch(pyramid_fragColor, ivec2(+1,  0) + ivec2(gl_FragCoord.xy) / 2, pyramid_level + 1);
            fragColor += c * 3.0/16.0;
            c = texelFetch(pyramid_fragColor, ivec2( 0, +1) + ivec2(gl_FragCoord.xy) / 2, pyramid_level + 1);
            fragColor += c * 3.0/16.0;
            c = texelFetch(pyramid_fragColor, ivec2(+1, +1) + ivec2(gl_FragCoord.xy) / 2, pyramid_level + 1);
            fragColor += c * 1.0/16.0;
        }
    }
}