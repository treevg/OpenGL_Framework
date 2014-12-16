#version 430

uniform sampler2D color;

in vec4 passPosition;
//in vec3 col;

out vec4 fragColor;

void main() {

	fragColor = texture(color, passPosition.xy);
	// fragColor = vec4(0,0,1,1);

}