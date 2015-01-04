#version 430

uniform sampler2D color;
//uniform float warpView;

in vec2 passPosition;
in vec4 passColor;

out vec4 fragColor;
//out vec4 fragColor2;

void main() {

	fragColor = texture(color, passPosition);
	
	
	//fragColor = passColor;
	// fragColor = vec4(0,0,1,1);

}