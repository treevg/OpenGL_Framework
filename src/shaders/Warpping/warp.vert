#version 430

uniform mat4 		viewMat;
uniform int 		switchWarp;
uniform mat4		invViewProjection;
uniform mat4		projection;
uniform sampler2D 	colorTexture;
uniform sampler2D 	positionTexture;

in vec2 pos;

out vec4 tempColor;

void main() {

	if(switchWarp==0){
		gl_Position = projection * viewMat * texture(positionTexture, pos);
		tempColor = texture(colorTexture,pos);
	} else {
		gl_Position = vec4(pos * 2 - 1, 0, 1);
		tempColor = texture(colorTexture, pos);
	}
}