#version 430
 
layout(location = 0) in vec4 positionAttribute;
layout(location = 1) in vec3 normalAttribute;
layout(location = 2) in vec2 uvAttribute;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 passColor;
out vec2 passUv;

void main(){	
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * positionAttribute; 
  
	passUv = uvAttribute;
}