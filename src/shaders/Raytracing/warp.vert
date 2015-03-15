#version 430

in vec2 pos;

uniform mat4 		altView;
uniform mat4		invViewProjection;
uniform mat4		projection;
uniform sampler2D 	diffuseDepthTexture;
uniform sampler2D 	diffPositionTexture;
uniform sampler2D 	normalTexture;

out vec2 passPosition;
//out vec4 warpedNormal;
out vec4 warpedDiffPos;  

void main() {
	//mat4 viewProj = projection * altView;
	
	float diffDepth_old = texture(diffuseDepthTexture, pos).x;
	
	//vec3 normal_old = (invViewProjection * vec4(texture(normalTexture, pos).xyz, 0)).xyz;
	vec3 normal_old = texture(normalTexture, pos).xyz;
	
	passPosition = pos;
	
	//warpedNormal =  vec4((projection * altView * vec4(normal_old, 0)).xyz, 0);
	
	// correct?
	gl_Position = vec4(pos * 2 - 1, 0, 1);
	//warpedNormal =  altView * vec4(normal_old, 0);  


	if(diffDepth_old >= 999){
		vec4 diffPos_old = invViewProjection * vec4(pos * 2 - 1, 0.999, 1);
		gl_Position = projection * altView * diffPos_old;
		warpedDiffPos = projection * altView * diffPos_old;
	}
	else {
		vec4 diffPos_old = texture(diffPositionTexture, pos);
		gl_Position = projection * altView * diffPos_old;
		warpedDiffPos = projection * altView * diffPos_old;

	}	
}



