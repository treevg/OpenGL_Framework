#version 430

in vec2 passPosition;
in vec4 warpRefPos;
in vec2 outCoord;

out vec4 warpedReflectivePosition;
out vec2 splattedRefUV;


void main() {
	warpedReflectivePosition = warpRefPos;
	splattedRefUV = outCoord;
}