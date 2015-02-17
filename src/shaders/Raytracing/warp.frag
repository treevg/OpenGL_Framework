#version 430

//uniform sampler2D color;

in vec2 passPosition;
//in vec4 passColor;
//in vec4 passIndColor;

in vec4 tempColor;
out vec4 fragColor;

void main() {
	fragColor = tempColor;
	//fragColor = texture(color, passPosition);
	//fragColor = passColor + passIndColor;


}