#version 430

in vec2 passPosition;

uniform vec2 resolution;
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D 	diffPositionTexture;

out vec4 position;
out vec4 diffuse;
out vec4 normal; 
out vec4 uv; 
out vec4 flow;

void main() {
	uv = vec4(passPosition, 0, 1);
	position = texture(diffPositionTexture, passPosition); 
	diffuse	= texture(diffuseTexture, passPosition);
	normal	= texture(normalTexture, passPosition);
	flow = vec4(gl_FragCoord.xy / resolution - passPosition, 0, 0);
}