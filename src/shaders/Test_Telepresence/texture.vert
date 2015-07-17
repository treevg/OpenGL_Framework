#version 430
 
layout(location = 0) in vec4 positionAttribute;
layout(location = 1) in vec3 normalAttribute;
layout(location = 2) in vec2 uvAttribute;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform int spherical=1; // 1 for spherical; 0 for cylindrical

out vec3 passColor;
out vec2 passUv;

void main(){
//gl_Position = projectionMatrix * viewMatrix * modelMatrix * positionAttribute;
	mat4 modelViewMatrix = viewMatrix * modelMatrix;
	
	  // First colunm.
  modelViewMatrix[0][0] = 1.0; 
  modelViewMatrix[0][1] = 0.0; 
  modelViewMatrix[0][2] = 0.0; 
 
//  if (spherical == 1)
//  {
    // Second colunm.
    modelViewMatrix[1][0] = 0.0; 
    modelViewMatrix[1][1] = 1.0; 
    modelViewMatrix[1][2] = 0.0; 
//  }
 
  // Thrid colunm.
  modelViewMatrix[2][0] = 0.0; 
  modelViewMatrix[2][1] = 0.0; 
  modelViewMatrix[2][2] = 1.0; 
  
  gl_Position = projectionMatrix * modelViewMatrix * positionAttribute;
  // vec4 point = modelViewMatrix * positionAttribute;
  //gl_Position = projectionMatrix * point;
  
passUv = uvAttribute;
}