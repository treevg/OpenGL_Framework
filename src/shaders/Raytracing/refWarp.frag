#version 430

uniform sampler2D reflectiveColorTexture;

in vec4 passPosition;
in vec4 warpRefPos;
in vec2 coordColor;

//out vec4 warpedReflectivePosition;
out vec2 splattedRefUV;
out vec4 position;
out vec4 refColor;

void main() {
	position = vec4(passPosition.xyz / passPosition.a, 1);
	vec4 uv = vec4((position.xy + 1) * 0.5,0,0); 
	refColor = texture(reflectiveColorTexture, uv.xy);
	
	//warpedReflectivePosition = warpRefPos;
	splattedRefUV = coordColor;

}