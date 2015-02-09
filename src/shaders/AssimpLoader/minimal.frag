#version 330

uniform vec4 color;
uniform float luminance;
uniform vec4 newColor;
uniform float bla;

in vec4 passPosition;
in vec2 passUV;

out vec4 fragColor;

void main()
{
    fragColor = vec4(passPosition.xyz, 1.0);
}