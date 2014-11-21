#version 430

// a simple vertex shader which simply applies the MVP matrix to the vertex and passes it to the fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec4 positionAttribute;
layout (location = 1) in vec2 uvCoordAttribute;

out vec2 passUV;

void main() {
	// vertex position
	gl_Position = projection * view * model * positionAttribute;
    passUV = uvCoordAttribute;
}