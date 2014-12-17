#version 430

uniform sampler2D color;
//uniform float warpView;

in vec4 passPosition;
in vec3 col;

out vec4 fragColor;
//out vec4 fragColor2;

void main() {

	//fragColor = texture(color, passPosition.xy);
	
	fragColor = vec4(col, 1);
	
	// fragColor = vec4(0,0,1,1);
	//fragColor2 = vec4(1,0,0,1);

}