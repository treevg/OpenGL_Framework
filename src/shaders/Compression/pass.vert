#version 430

in vec4 pos;

out vec4 passPosition;

void main() {
	passPosition = pos;
    gl_Position = vec4(pos.xy, 0, 1);
}
