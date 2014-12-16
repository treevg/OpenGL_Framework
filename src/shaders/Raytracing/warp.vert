#version 430

//uniform mat4 altView;
//uniform mat4 invView;
//uniform sampler2D depth;
//uniform sampler2D color;

// vec2 instead of vec4 ?
in vec4 pos;

out vec4 passPosition;
//out vec3 col;

void main() {
//	col = texture(color,pos.xy).xyz;
//	float z = texture(depth,pos.xy).x;
//	vec4 w = invView*vec4(pos.xy, z,1);
	
	passPosition = pos;
	
	//gl_Position = altView*w;
	gl_Position = vec4(pos.xy * 2 - 1, 0, 1);
}