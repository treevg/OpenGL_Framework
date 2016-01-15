#version 430

out vec4 fragColor;

uniform sampler2D pyramid_fragColor;
uniform int pyramid_level;


float sum = 0;

void addTexel(int offxetX, int offsetY, float factor) {
    vec4 c = texelFetch(pyramid_fragColor, ivec2(offxetX,  offsetY) + ivec2(gl_FragCoord.xy) / 2, pyramid_level + 1);
    
    if (c.a == 0) { // if texel is undefined
        fragColor += c * factor;
        sum += factor;
    }
}

void main() {
    fragColor = vec4(0,0,0,0);

    addTexel(0, 0, 9.0);

    if (int(gl_FragCoord.x) % 2 == 0) {
        if (int(gl_FragCoord.y) % 2 == 0) {
            addTexel(-1,  0, 3.0);
            addTexel( 0, -1, 3.0);
            addTexel(-1, -1, 1.0);
        } else {
            addTexel(-1,  0, 3.0);
            addTexel( 0, +1, 3.0);
            addTexel(-1, +1, 1.0);
        }
    } else {
        if (int(gl_FragCoord.y) % 2 == 0) {
            addTexel(+1,  0, 3.0);
            addTexel( 0, -1, 3.0);
            addTexel(+1, -1, 1.0);
        } else {
            addTexel(+1,  0, 3.0);
            addTexel( 0, +1, 3.0);
            addTexel(+1, +1, 1.0);
        }
    }
    fragColor /= sum;
}