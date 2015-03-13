#version 430


in vec2 passPosition;
in vec4 warpedNormal;
in vec4 warpedDiffPos;

//in vec3 newViewDir;
//in vec4 diffColor;
//out vec3 newViewDirection;
//out vec4 diffCol;

out vec4 warpDiffPos;
out vec4 warpNormal; 


void main() {
//	newViewDirection	= newViewDir;
//	diffCol 	= diffColor;

	warpDiffPos = warpedDiffPos;
	warpNormal	= warpedNormal;

}