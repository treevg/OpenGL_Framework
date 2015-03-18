#version 330 core

layout (location = 0) in vec4 positionAttribute;
layout (location = 1) in vec3 normalAttribute;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 passPosition;
out vec3 passNormal;

void main(){
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * positionAttribute;

    //pass normal and position in viewspace
	passPosition = viewMatrix * modelMatrix * positionAttribute;
	vec4 n = transpose(inverse(viewMatrix * modelMatrix)) * vec4(normalAttribute, 0);
	passNormal = normalize(n.xyz);
}