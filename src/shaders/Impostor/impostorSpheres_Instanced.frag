#version 430

in vec2 texCoord;
in float depth;
in vec4 eye_pos;
in float size;
in vec4 passColor;
out vec4 fragColor;

#define PI = 3.1415926535897932384626433832795;
#define PIh = PI/2;
uniform mat4 projection;
uniform mat4 view;
layout(depth_less) out float gl_FragDepth;

uniform vec3 lightSrc = vec3(100,100,100);
uniform float cutPlaneDistance = 10.0f;
vec3 N;
vec3 Idiff;

void main() {
    if (length(texCoord) > 1) discard;
    ivec2 coords = ivec2(gl_FragCoord);

    float depthOffset = (sin(acos(length(texCoord.xy))));
    float scaledDepthOffset = depthOffset * size / 2;
    float modifiedDepth = depth - scaledDepthOffset;

    if (depth < cutPlaneDistance)
    {
        if (scaledDepthOffset < abs(depth - cutPlaneDistance)) discard;
    }

    gl_FragDepth = modifiedDepth / 1000.0f; // far plane is at 1000, near at 0.1
    if (modifiedDepth > cutPlaneDistance){
        vec4 normal = vec4(texCoord.xy, depthOffset,0);
        N = normalize(normal.xyz);
    }
    else
    {
        N = vec3(0,0,1);
    }

    vec4 corrected_pos = vec4(eye_pos.xy, eye_pos.z + depthOffset * size / 2, 1);
    vec3 light_eyePos = lightSrc.xyz;//vec3(view * vec4(lightSrc, 1)).xyz;
    vec3 L = normalize(vec3(light_eyePos - corrected_pos.xyz));

    Idiff = passColor.xyz * max(dot(N,L), 0.0);
    Idiff = clamp(Idiff, 0.0, 1.0);

    fragColor = vec4(Idiff, modifiedDepth / 1.0f);
}
