#version 430

uniform sampler2D color;

in vec2 passPosition;
//in vec3 col;

out vec4 fragColor;

void main() {

fragColor = texture(color, passPosition.xy);
	//fragColor = vec4(col,1);

}