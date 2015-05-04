#version 430

in vec2 pos;

uniform mat4 		view;
uniform mat4		projection;
uniform sampler2D 	diffPositionTexture;

out vec2 passPosition;
 
void main() {
	passPosition = pos;
	gl_Position = projection * view * texture(diffPositionTexture, pos) * vec4(1,1,0.999,1);	
}