#version 430

uniform mat4 altView;
uniform mat4 invView;
uniform sampler2D depth;
uniform sampler2D color;
uniform int warpView;

in vec2 pos;

out vec2 passPosition;
out vec4 passColor;

void main() {
	
	float z = texture(depth,pos).x;
	
	if(warpView==0){
		if(z>=999){z=0;}
		vec4 w	= invView*vec4(pos * 2 - 1, z,1);
		gl_Position = altView*w;
	}
	else{
		gl_Position = vec4(pos * 2 - 1, 0, 1);
	}
	passColor	= texture(color,pos);
	passPosition = pos;
	
}