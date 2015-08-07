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
	mat4 modelViewMatrix = viewMatrix * modelMatrix;
	
	modelViewMatrix[0][0] = 1.0; 
	modelViewMatrix[0][1] = 0.0; 
	modelViewMatrix[0][2] = 0.0; 
	
	modelViewMatrix[1][0] = 0.0; 
	modelViewMatrix[1][1] = 1.0; 
	modelViewMatrix[1][2] = 0.0;
	
	modelViewMatrix[2][0] = 0.0; 
	modelViewMatrix[2][1] = 0.0; 
	modelViewMatrix[2][2] = 1.0; 
	
	gl_Position = projectionMatrix * modelViewMatrix * positionAttribute; 
  
	passUv = uvAttribute;
}