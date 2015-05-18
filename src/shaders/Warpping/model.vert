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
out float visability;

const float density = 0.0025;
const float gradient = 4.0;


void main() {
	
	    vec4 posRelToCam = uniformView * uniformModel * vertex_modelSpace; 
	    gl_Position =  uniformProjection *  posRelToCam;
            vec4 temp_normal =  uniformModel * vec4(vertex_normal,0); 
	    float  distance = length(posRelToCam.xyz);
	    visability = exp(-pow((distance*density),gradient));
            visability = clamp(visability, 0.0, 1.0);
           
        passNormal = vec3(temp_normal.xyz);
        passUV = vertexUV;
        passPosition =  uniformModel * vertex_modelSpace;
     
}
