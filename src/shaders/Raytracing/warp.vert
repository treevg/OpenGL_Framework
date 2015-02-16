#version 430

uniform mat4 		altView;
uniform int 		warpView;
uniform mat4		invViewProjection;
uniform mat4		projection;
uniform sampler2D	indirectColor;
uniform sampler2D 	depth;
uniform sampler2D 	color;

in vec2 pos;

out vec2 passPosition;
//out vec4 passColor;
//out vec4 passIndColor;
out vec4 tempColor;

void main() {
	vec4 w;
	float z = texture(depth, pos);
	
	
	//vec4 passColor	= texture(color,pos);
	//vec4 passIndColor 	= texture(indirectColor, pos);

	
	if(warpView==0){
		if(z>=999){
			
			w =   invViewProjection * vec4(pos * 2 - 1, 0.999, 1);
			gl_Position = projection * altView * w;
			tempColor = texture(color,pos) + texture(indirectColor, pos);
			
		}
		else {
			w	= invViewProjection * vec4(pos * 2 - 1, z,1);
			gl_Position = projection * altView * w;
			tempColor = texture(color,pos)* texture(indirectColor, pos);
		}	
	}
	
	else{
		gl_Position = vec4(pos * 2 - 1, 0, 1);
		tempColor = texture(color, pos) + (texture(indirectColor,pos)-0.2);
	}

	passPosition = pos;
	
}