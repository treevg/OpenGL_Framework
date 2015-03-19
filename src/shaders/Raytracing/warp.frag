#version 430

in vec4 passPosition;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;

out vec4 position;
out vec4 diffuse;
out vec4 normal; 
out vec4 uv; 

void main() {
	position = vec4(passPosition.xyz / passPosition.a, 1);
	// uv = passPosition; //todo
	// diffuse	= texture(diffuseTexture, uv.xy);
	// normal	= texture(normalTexture, uv.xy);

	position = vec4(position.xy,0,1);
	uv = vec4(position.xy,0,1);
	diffuse	= vec4(position.xy,0,1);
	normal	= vec4(position.xy,0,1);
}