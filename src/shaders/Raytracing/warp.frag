#version 430

in vec4 passPosition;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;

out vec4 position;
out vec4 diffuse;
out vec4 normal; 
out vec4 uv; 

void main() {
	// position = passPosition;
	// uv = passPosition; //todo
	// diffuse	= texture(diffuseTexture, uv.xy);
	// normal	= texture(normalTexture, uv.xy);

	vec4 bla = vec4(passPosition.xy,0,0);
	position = bla;
	uv = bla;
	diffuse	= bla;
	normal	= bla;
}