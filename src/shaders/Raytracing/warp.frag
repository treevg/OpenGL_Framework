#version 430

in vec2 passPosition;
in vec4 diffColor;
in vec4 warpedNormal;
in vec4 warpedDiffPos;
in vec2 coordColor;
in vec4 reflectionPosition;

in vec3 newViewDir;
in vec4 iPos;
//in vec4 outCoo;

in vec2 outCoord;

out vec3 newViewDirection;
out vec4 diffCol;
//out vec4 coord;

out vec2 outCo;

out vec4 warpDiffPos;
out vec4 warpNormal; 
out vec2 coordCol;
out vec4 refPos;


void main() {
	newViewDirection	= newViewDir;
	diffCol 	= diffColor;
	warpDiffPos = warpedDiffPos;
	warpNormal	= warpedNormal;
	coordCol 	= coordColor;
	refPos 		= iPos;  //reflectionPosition;
	
	//coord = outCoo;
	outCo= outCoord;
}