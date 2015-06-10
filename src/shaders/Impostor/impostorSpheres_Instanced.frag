#version 430

in vec2 texCoord;
in float depth;
in vec4 eye_pos;
in float size;
in vec4 color;
out vec4 fragColor;

#define PI = 3.1415926535897932384626433832795;
#define PIh = PI/2;
uniform sampler2D tex;
uniform mat4 projection;
uniform mat4 view;

uniform vec3 lightSrc = vec3(100,100,100);

void main() {
    vec4 texel = texelFetch(tex, ivec2(gl_FragCoord), 0);
    if (length(texCoord) > 1) discard;

    float depthOffset = (sin(acos(length(texCoord.xy))));
    float scaledDepthOffset = depthOffset * size / 2;
    float modifiedDepth = depth - scaledDepthOffset;

    if (modifiedDepth < texel.w){
        vec4 normal = vec4(texCoord.xy, depthOffset,0);
        vec3 N = normalize(normal.xyz);

        vec4 corrected_pos = vec4(eye_pos.xy, eye_pos.z + depthOffset * size / 2, 1);
        vec3 light_eyePos = vec3(view * vec4(lightSrc, 1)).xyz;
        vec3 L = normalize(vec3(light_eyePos - corrected_pos.xyz));

        vec3 Idiff = color.xyz * max(dot(N,L), 0.0);
        Idiff = clamp(Idiff, 0.0, 1.0);

        fragColor = vec4(Idiff, modifiedDepth);
    }
    else
        fragColor = texel;
}
