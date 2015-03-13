#version 430

uniform mat4 		altView;
uniform mat4		invViewProjection;
uniform mat4		projection;
uniform sampler2D 	diffuseDepthTexture;
uniform sampler2D 	diffPositionTexture;
uniform sampler2D 	normalTexture;


in vec2 pos;

out vec2 passPosition;
out vec4 warpedNormal;
out vec4 warpedDiffPos;

  

void main() {
	//mat4 viewProj = projection * altView;
	
	vec4 diffPos_old;
	float diffDepth_old = texture(diffuseDepthTexture, pos).x;
	//vec3 normal_old = (invViewProjection * vec4(texture(normalTexture, pos).xyz, 0)).xyz;
	vec3 normal_old = texture(normalTexture, pos).xyz;
	
	passPosition = pos;
	
	//warpedNormal =  vec4((projection * altView * vec4(normal_old, 0)).xyz, 0);
	warpedNormal =  vec4((altView * vec4(normal_old, 0)).xyz, 0);

	gl_Position = vec4(pos * 2 - 1, 0, 1);
	
	if(diffDepth_old >= 999){
		diffPos_old =   invViewProjection * vec4(pos * 2 - 1, 0.999, 1);
		warpedDiffPos = projection * altView * diffPos_old;
		gl_Position = altView * diffPos_old;
		
		//gl_Position = projection * altView * diffPos_old;
		//diffColor = texture(colorTexture,pos) + texture(indirectColorTexture, pos);
	}
	
	else {
		 vec4 diffPos_old	= texture(diffPositionTexture, pos);
		warpedDiffPos = projection * altView * diffPos_old;
		
		//gl_Position = altView * diffPos_old;
		
		
		//gl_Position = projection * altView * diffPos_old;	
		//diffColor = texture(colorTexture,pos);
	}	

}

