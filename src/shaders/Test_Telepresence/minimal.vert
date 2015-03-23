#version 430
 
layout(location = 0) in vec3 positionAttribute;
layout(location = 1) in vec3 colorAttribute;

uniform mat4 view;
uniform mat4 projection;

out vec4 passPosition;
out vec3 passColor;

void main(){
	passPosition = view * vec4(positionAttribute, 1.0);
	passColor = colorAttribute;
	gl_Position =  projection * view * vec4(positionAttribute, 1.0);
}