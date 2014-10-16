#version 430

uniform sampler2D tex;

in vec4 passPosition;

out vec4 fragColor;

void main() {
    fragColor = texture(tex, passPosition.xy);
}