#version 430

uniform mat4 		altView;
uniform mat4 		rotMat;
uniform mat4 		view;
uniform sampler2D 	depth;
uniform sampler2D 	color;
uniform int 		warpView;
uniform mat4		invViewProjection;
uniform mat4		projection;
uniform mat4		rotationOnly;
//uniform sampler2D	extraDepth;


in vec2 pos;

out vec2 passPosition;
out vec4 passColor;

void main() {
	vec4 w;
	float z = texture(depth, pos);
	//float d = texture(extraDepth, pos);

	
	mat4 exD = 
	mat4(1, 0, 0, 0,
	     0, 1, 0, 0,
	     0, 0, 1, 0,
	     0, 0, 0, 1 );
	
	
	
	if(warpView==0){
		if(z>=999){
			
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