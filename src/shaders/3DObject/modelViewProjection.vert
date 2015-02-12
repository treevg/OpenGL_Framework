#version 430
 
layout(location = 0) in vec4 positionAttribute;
layout(location = 1) in vec2 uvCoordAttribute;
layout(location = 2) in vec4 normalAttribute;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 passPosition;
out vec2 passUVCoord;
out vec3 passNormal;

void main(){
    passUVCoord = uvCoordAttribute;

    passPosition = view * model * positionAttribute;
    gl_Position =  projection * view * model * positionAttribute;

	passNormal = ( transpose( inverse( view * model ) ) * normalAttribute ).xyz;
}