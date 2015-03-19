#version 430

in vec2 passPosition;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D 	diffPositionTexture;

out vec4 position;
out vec4 diffuse;
out vec4 normal; 
out vec4 uv; 

void main() {
	position = vec4(passPosition, 0, 1);
	uv = texture(diffPositionTexture, passPosition); 
	diffuse	= texture(diffuseTexture, passPosition);
	normal	= texture(normalTexture, passPosition);
}