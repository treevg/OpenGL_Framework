#version 430

// attention, this is a dumb and slow method for presentation purpose only

out vec4 fragColor;

uniform sampler2D tex;
uniform int strength = 4;

void main() {
    fragColor = vec4(0,0,0,0);
    float count = 0;

    for (int i = -strength; i <= strength; i++) {
        for (int j = -strength; j <= strength; j++) {
            count++;
            fragColor += texelFetch(tex, ivec2(gl_FragCoord.xy) + ivec2(i, j), 0);
        }
    }
    fragColor /= count;
}