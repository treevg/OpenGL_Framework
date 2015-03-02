#version 430

layout (location = 0) in vec4 vertex_modelSpace;
layout (location = 1) in vec2 textureCoordinates;
layout (location = 2) in vec3 normal;

uniform mat4 uniformModel;
uniform mat4 uniformView;
uniform mat4 uniformProjection;

out vec4 passPosition;
out vec2 passTextureCoordinate;
out vec3 passNormal;



void main() {

	passPosition =  uniformModel * vertex_modelSpace ;
        gl_Position =  uniformProjection *  uniformView * uniformModel *vertex_modelSpace ;
        passTextureCoordinate = textureCoordinates;
        passNormal = normal;
   
}
