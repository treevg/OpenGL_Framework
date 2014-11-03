#version 330

in vec2 passUV;

uniform sampler2D tex;

out vec4 fragmentColor;

void main() {
    fragmentColor = texture(tex, passUV);
}