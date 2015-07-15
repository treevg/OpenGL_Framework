#version 430

in vec4 pos;

out vec3 passPosition;
out vec2 texCoord;

void main() {
	passPosition = pos.xyz;
        texCoord = pos.xy;
        gl_Position = vec4(pos.xy * 2 - 1, 0, 1);
}
