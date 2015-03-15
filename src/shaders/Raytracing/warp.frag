#version 430

in vec2 passPosition;
//in vec4 warpedNormal;
in vec4 warpedDiffPos;

out vec4 warpDiffPos;
//out vec4 warpNormal; 

void main() {
	warpDiffPos = warpedDiffPos;
//	warpNormal	= warpedNormal;

}