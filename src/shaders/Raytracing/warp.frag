#version 430

uniform sampler2D color;

in vec2 passPosition;
in vec4 passColor;

out vec4 fragColor;

void main() {

	fragColor = texture(color, passPosition);
	//fragColor = passColor;


}