#version 430

in vec4 pos;

out vec4 passPosition;
out vec2 passUV;

void main() {
	passPosition = pos;
	passUV = (pos.xy + 1.0) / 2.0;
    gl_Position = vec4(pos.xy, 0, 1);
}
