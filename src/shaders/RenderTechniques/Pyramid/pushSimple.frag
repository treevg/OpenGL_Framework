#version 430

out vec4 fragColor;
out vec4 fragPosition;

uniform sampler2D pyramid_fragColor;
uniform sampler2D pyramid_fragPosition;
uniform sampler2D input_image;
uniform int level;
uniform vec2 resolution;

void main() {
    if (level == 0) {
        fragColor = texelFetch(input_image, ivec2(gl_FragCoord.xy), 0);
        fragPosition = vec4(vec2(gl_FragCoord.xy) / resolution, 0, 1);
    } else {
        fragColor = texelFetch(pyramid_fragColor, ivec2(gl_FragCoord.xy) * 2, level-1);
        fragPosition = texelFetch(pyramid_fragPosition, ivec2(gl_FragCoord.xy) * 2, level-1);
    }
}