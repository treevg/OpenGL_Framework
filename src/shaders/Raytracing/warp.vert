#version 430

uniform mat4 		altView;
uniform mat4 		view;
uniform sampler2D 	depth;
uniform sampler2D 	color;
uniform int 		warpView;
uniform mat4		invViewProjection;
uniform mat4		projection;
uniform mat4		rotationOnly;

in vec2 pos;

out vec2 passPosition;
out vec4 passColor;

void main() {
	vec4 w;
	float z = texture(depth, pos).x;
	
	if(warpView==0){
		if(z>=999.0){
			
			w =   invViewProjection * vec4(pos * 2 - 1, 0.999, 1);
			gl_Position = projection * altView * w;
			
		}
		
		else {
			w	= invViewProjection * vec4(pos * 2 - 1, z,1);
			gl_Position = projection * altView * w;
		}
		
		
	}
	
	else{
		gl_Position = vec4(pos * 2 - 1, 0, 1);
	}
	passColor	= texture(color,pos);
	passPosition = pos;
	
}