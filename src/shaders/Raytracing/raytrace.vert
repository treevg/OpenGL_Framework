#version 430

in vec2 pos;

out vec2 passPosition;

void main() {
	passPosition = pos;
	gl_Position = vec4(pos, 0, 1);
}
