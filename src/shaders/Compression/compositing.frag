#version 430

uniform sampler2D tex2;

in vec4 passPosition;

out vec4 fragColor;

void main() {
    fragColor = texture(tex2, passPosition.xy);
}