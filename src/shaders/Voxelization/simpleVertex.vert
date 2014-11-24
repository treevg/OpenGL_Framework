#version 430

// a simple vertex shader which simply applies the MVP matrix to the vertex and passes it to the fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float near;
uniform float far;

layout (location = 0) in vec4 positionAttribute;
layout (location = 1) in vec2 uvCoordAttribute;

out vec2 passUV;

out float clipDepth;

void main() {
	// vertex position
	vec4 viewpos = view * model * positionAttribute;
	gl_Position = projection * viewpos;
    passUV = uvCoordAttribute;
    
    // distance to camera
    clipDepth = (viewpos.z - near) / (far - near);
}