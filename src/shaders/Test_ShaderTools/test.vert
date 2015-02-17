#version 430

in vec4 pos;


uniform mat4 uniformView;
uniform mat4 uniformModel;
uniform mat4 uniformProjection;

out vec4 passPosition;

void main() {
	passPosition = uniformModel*pos;
        gl_Position = uniformProjection*uniformView*uniformModel*pos;
}
