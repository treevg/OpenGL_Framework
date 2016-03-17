#version 430
 
layout(location = 4) in vec3 positionAttribute;
layout(location = 5) in vec3 colorAttribute;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out VertexData
{
	vec4 vertexPosition;
	vec3 vertexColor;
}  vertex;

void main(){
	if( positionAttribute == vec3(1.0))
	{
		gl_Position =  vec4(0.0);
	}
	else
	{
		vertex.vertexPosition = viewMatrix * vec4(positionAttribute, 1.0);
		vertex.vertexColor = colorAttribute;
		gl_Position =  projectionMatrix * viewMatrix * vec4(positionAttribute, 1.0);
	}
}