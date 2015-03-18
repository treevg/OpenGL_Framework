#version 430

in vec2 passPosition;
in vec4 warpedNormal;

out vec4 warpNormal; 

void main() {
	warpNormal	= warpedNormal;
}