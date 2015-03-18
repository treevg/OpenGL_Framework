#version 430

in vec2 pos;

uniform mat4 		altView;
uniform mat4		invViewProjection;
uniform mat4		projection;
uniform sampler2D 	normalTexture;
uniform sampler2D	diffusePositionTexture;

out vec2 passPosition;
out vec4 warpedNormal; 

void main() {
	vec3 normal_old = texture(normalTexture, pos).xyz;
	passPosition = pos;
	vec4 diffPos_old = invViewProjection * vec4(pos * 2 - 1, 0.999, 1);
	gl_Position = projection * altView * diffPos_old;
	warpedNormal =   vec4(normal_old, 0);
}



