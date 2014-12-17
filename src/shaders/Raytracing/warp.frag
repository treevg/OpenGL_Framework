#version 430

uniform sampler2D color;
uniform float bla;

in vec4 passPosition;
//in vec3 col;

out vec4 fragColor;
out vec4 fragColor2;

void main() {

	fragColor = texture(color * bla, passPosition.xy);
	// fragColor = vec4(0,0,1,1);
	fragColor2 = vec4(1,0,0,1);

}