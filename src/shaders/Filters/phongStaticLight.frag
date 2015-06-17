#version 430

out vec4 fragColor;

in vec4 passColor;
in vec4 passWorldPosition;
in vec3 passWorldNormal;

uniform vec3 lightSrc = vec3(100,100,100);

void main() {
    vec3 N = passWorldNormal;
    vec3 L = normalize(vec3(lightSrc - passWorldPosition.xyz));

    vec3 Idiff = passColor.xyz * max(dot(N,L), 0.0);
    Idiff = clamp(Idiff, 0.0, 1.0);

    fragColor = vec4(Idiff,0);
}
