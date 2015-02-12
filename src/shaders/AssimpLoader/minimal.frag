#version 440

in vec4 passPosition;
in vec2 passUV;
in vec4 passNormal;
in vec3 passColor;

out vec4 fragColor;

void main()
{
    fragColor = vec4(passPosition.xyz, 1.0);
}