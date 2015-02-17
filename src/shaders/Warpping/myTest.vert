#version 430

layout (location = 0) in vec4 vertex_modelSpace;
//layout (location = 1) in vec2 vertexUV;

uniform mat4 uniformModel;
uniform mat4 uniformView;
uniform mat4 uniformProjection;

out vec4 passPosition;
//out vec2 passUV;


void main() {
	 	passPosition =  uniformModel * vertex_modelSpace ;
        gl_Position =  uniformProjection *  uniformView * uniformModel *vertex_modelSpace ;
        // passUV = vertexUV;
   
}
