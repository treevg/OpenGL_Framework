#version 430

out vec4 fragColor;

uniform sampler2D inputTex;
uniform sampler2D pyramid_fragColor;
uniform int pyramid_level;

// float[4][4] kernel = { // sigma 1
//     { 0.04916008,  0.06170028,  0.06170028,  0.04916008 },
//     { 0.06170028,  0.07743935,  0.07743935,  0.06170028 },
//     { 0.06170028,  0.07743935,  0.07743935,  0.06170028 },
//     { 0.04916008,  0.06170028,  0.06170028,  0.04916008 }};


float[4][4] kernel = { // sigma 3
    { 0.00908864,  0.03857854,  0.03857854,  0.00908864 },
    { 0.03857854,  0.16375428,  0.16375428,  0.03857854 },
    { 0.03857854,  0.16375428,  0.16375428,  0.03857854 },
    { 0.00908864,  0.03857854,  0.03857854,  0.00908864 }};


void main() {
    if (pyramid_level == 0) {
        fragColor = texelFetch(inputTex, ivec2(gl_FragCoord.xy), 0);
        return;
    }

    ivec2 coord = ivec2(gl_FragCoord.xy * 2 - 1);
    
    fragColor = vec4(0,0,0,0);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            vec4 c = texelFetch(pyramid_fragColor, ivec2(i, j) - 1 + coord, pyramid_level - 1);
            fragColor += c * kernel[i][j];
        }   
    }
}