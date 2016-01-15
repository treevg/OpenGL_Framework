#version 430

out vec4 fragColor;
out vec4 fragPosition;

uniform sampler2D inputTex;
uniform sampler2D pyramid_fragColor;
uniform sampler2D pyramid_fragPosition;
uniform int pyramid_level;
uniform vec2 resolution;

void main() {
    if (pyramid_level == 0) {
        fragColor = texelFetch(inputTex, ivec2(gl_FragCoord.xy), 0);
        fragPosition = vec4(vec2(gl_FragCoord.xy) / resolution, 0, 1);
    } else {
        fragColor = texelFetch(pyramid_fragColor, ivec2(gl_FragCoord.xy) * 2, pyramid_level-1);
        fragPosition = texelFetch(pyramid_fragPosition, ivec2(gl_FragCoord.xy) * 2, pyramid_level-1);
    }
}