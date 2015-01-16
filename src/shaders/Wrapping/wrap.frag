#version 430

uniform vec4 color;
uniform float luminance;

in vec4 passPosition;

out vec4 fragColor;
out vec4 fragPosition;

void main() {
    fragColor = color * luminance;
    fragPosition = passPosition;
}
