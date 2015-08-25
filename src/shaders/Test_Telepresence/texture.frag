#version 430

uniform sampler2D tex;
in vec2 passUv;

out vec4 fragColor;

void main() {
	fragColor = texture(tex, passUv.xy);
}