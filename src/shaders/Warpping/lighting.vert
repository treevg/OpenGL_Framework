#version 430

layout (location = 0) in vec4 vertex_modelSpace;
layout (location = 1 ) in vec3 vertex_normal;
layout (location = 2) in vec2 textureCoordinates;


uniform mat4 uniformModel;
uniform mat4 uniformView;
uniform mat4 uniformProjection;


out vec4 passPosition;
out vec3 passNormal;
out vec2 passTextureCoordinate;


void main() {

        gl_Position = uniformProjection * uniformView *  uniformModel * vertex_modelSpace; 
        vec4 temp_normal = transpose(inverse(uniformView * uniformModel)) * vec4(vertex_normal,0); 
           
        passNormal = vec3(temp_normal.xyz);
        passTextureCoordinate = textureCoordinates;
        passPosition =  uniformView * uniformModel * vertex_modelSpace;
  
}
