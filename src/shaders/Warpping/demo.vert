#version 430

layout (location = 0) in vec4 vertex_modelSpace;
layout (location = 1) in vec2 textureCoordinates;


uniform mat4 uniformModel;
uniform mat4 uniformView;
uniform mat4 uniformProjection;

out vec4 passPosition;
out vec2 passTextureCoordinate;
out float visability;

const float density = 0.0035;
const float gradient = 4.0;


void main() {

	    passPosition =  uniformModel * vertex_modelSpace ;
	    vec4 posRelToCam = uniformView * uniformModel * vertex_modelSpace; 
	    gl_Position =  uniformProjection *  posRelToCam;
	    float  distance = length(posRelToCam.xyz);
            visability = exp(-pow((distance*density),gradient));
            visability = clamp(visability, 0.0, 1.0);
            passTextureCoordinate = textureCoordinates;
       
   
}
