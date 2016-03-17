#version 430

in vec4 passPosition;
in vec3 passColor;

out vec4 fragColor;

void main() {
	fragColor = vec4(passColor, passPosition.z);
}