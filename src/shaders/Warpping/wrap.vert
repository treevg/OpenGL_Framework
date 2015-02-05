#version 430

layout (location = 0) in vec4 vertex_modelSpace;

uniform mat4 uniformModel;
uniform mat4 uniformView;
uniform mat4 uniformProjection;


out vec4 passPosition;


void main() {
	    passPosition = uniformView * uniformModel *vertex_modelSpace ;
            gl_Position =  uniformProjection *  uniformView * uniformModel *vertex_modelSpace ;
   
}
