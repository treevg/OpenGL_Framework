#version 430

out vec4 fragColor;
in vec4 passColor;

void main() {
    fragColor = passColor;
}
