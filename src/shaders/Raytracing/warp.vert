#version 430

uniform mat4 		altView;
uniform sampler2D 	depth;
uniform sampler2D 	color;
uniform int 		warpView;
uniform mat4		invViewProjection;
uniform mat4		projection;
uniform mat4		rotationOnly;
uniform mat4 		altinvViewProjection;
in vec2 pos;

out vec2 passPosition;
out vec4 passColor;

void main() {
	vec4 w;
	float z = texture(depth, pos).x;
	
	if(warpView==0){
		if(z>=999){
			//mat4 altinvViewProjection = inverse(projection * view * rotationOnly);
			w =   invViewProjection * vec4(pos * 2 - 1, z, 1);
		}
		
		else {
			w	= invViewProjection * vec4(pos * 2 - 1, z,1);
		}
		
		gl_Position = projection * altView * w;
	}
	
	else{
		gl_Position = vec4(pos * 2 - 1, 0, 1);
	}
	passColor	= texture(color,pos);
	passPosition = pos;
	
}