#version 430

uniform mat4 		altView;
uniform int 		warpOnOff;
uniform mat4		invViewProjection;
uniform mat4		projection;
uniform sampler2D	indirectColorTexture;
uniform sampler2D 	depthTexture;
uniform sampler2D 	colorTexture;
uniform sampler2D 	positionTexture;
uniform sampler2D 	pixelNormalTexture;

in vec2 pos;

out vec2 passPosition;
//out vec4 passColor;
//out vec4 passIndColor;
out vec4 tempColor;

void main() {
	vec4 w;
	float z = texture(depthTexture, pos).x;

	//vec4 passColor	= texture(color,pos);
	//vec4 passIndColor 	= texture(indirectColorTexture, pos);

	
	if(warpOnOff==0){
		if(z>=999){
			
			w =   invViewProjection * vec4(pos * 2 - 1, 0.999, 1);
			gl_Position = projection * altView * w;
			tempColor = texture(colorTexture,pos) + texture(indirectColorTexture, pos);
			
		}
		else {
			//w	= invViewProjection * vec4(pos * 2 - 1, z, 1);
			w	= texture(positionTexture, pos);
			
			gl_Position = projection  * altView *  w ;
			tempColor = texture(colorTexture,pos) + texture(indirectColorTexture, pos);
		}	
	}
	
	else{
		gl_Position = vec4(pos * 2 - 1, 0, 1);
		tempColor =texture(pixelNormalTexture, pos) ;// + (texture(indirectColorTexture,pos)-0.2);
	}

	passPosition = pos;
	
}