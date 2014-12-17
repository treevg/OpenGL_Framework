#version 430

uniform mat4 altView;
uniform mat4 invView;
uniform sampler2D depth;
uniform sampler2D color;
uniform int warpView;
// vec2 instead of vec4 ?
in vec4 pos;

out vec4 passPosition;
out vec3 col;

void main() {
	col 	= texture(color,pos.xy).xyz;
	float z = texture(depth,pos.xy).x;
	
	if(warpView==0){
		vec4 w 	= invView*vec4(pos.xy * 2 - 1, z,1);
		gl_Position = altView*w;
	}
	else{
		gl_Position = vec4(pos.xy * 2 - 1, 0, 1);
	}
	
	passPosition = pos;
	
}