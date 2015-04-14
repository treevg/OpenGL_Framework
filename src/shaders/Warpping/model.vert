#version 430

layout (location = 0) in vec4 vertex_modelSpace;
layout (location = 1) in vec2 vertexUV;
layout (location = 2 ) in vec3 vertex_normal;

uniform mat4 uniformModel;
uniform mat4 uniformView;
uniform mat4 uniformProjection;


out vec4 passPosition;
out vec2 passUV;
out vec3 passNormal;


void main() {
	    gl_Position =  uniformProjection *  uniformView * uniformModel *vertex_modelSpace ;
            vec4 temp_normal = transpose(inverse(uniformView * uniformModel)) * vec4(vertex_normal,0); 
           
        passNormal = vec3(temp_normal.xyz);
        passUV = vertexUV;
        passPosition = uniformModel * vertex_modelSpace ;
     
}
