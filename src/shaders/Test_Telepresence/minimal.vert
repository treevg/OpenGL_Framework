#version 430
 
layout(location = 4) in vec3 positionAttribute;
layout(location = 5) in vec3 colorAttribute;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 passPosition;
out vec3 passColor;

void main(){
	passPosition = viewMatrix * vec4(positionAttribute, 1.0);
	//passColor = colorAttribute;
	passColor = vec3(positionAttribute.x,positionAttribute.y,positionAttribute.z);
	gl_Position =  projectionMatrix * viewMatrix * vec4(positionAttribute, 1.0);
}