#version 430

layout (location = 0) in vec4 positionAttribute;
layout (location = 1) in vec2 vertexUV;

in vec4 pos;

uniform float scale;

uniform mat4 uniformModel;
uniform mat4 uniformView;
uniform mat4 uniformProjection;

out vec4 passPosition;
out vec2 passUVCoord;

void main() {
	passPosition = uniformView * uniformModel * positionAttribute;
    gl_Position = uniformProjection * uniformView * uniformModel * positionAttribute;
    passUVCoord = vertexUV;
}
