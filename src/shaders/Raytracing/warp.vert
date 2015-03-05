#version 430

uniform mat4 		altView;
uniform int 		warpOnOff;
uniform mat4		invViewProjection;
uniform mat4		projection;
uniform sampler2D	indirectColorTexture;
uniform sampler2D 	depthTexture;
uniform sampler2D 	colorTexture;
uniform sampler2D 	diffPositionTexture;
uniform sampler2D 	normalTexture;
uniform sampler2D 	viewDirTexture;
uniform sampler2D 	depth2Texture;

//uniform sampler2D 	reflectedPositionTexture;
//uniform vec3 		initialPos;

layout ( rgba16f, binding = 0 ) uniform image2D texout;

in vec2 pos;

out vec2 passPosition;
out vec4 diffColor;  
out vec4 warpedNormal;
out vec4 warpedDiffPos;
out vec2 coordColor;
out vec4 reflectionPosition;


out vec3 newViewDir;
out vec4 iPos;
//out vec4 coordbla
//out vec2 outCoord;


void main() {
	vec4 diffPos_old;
	vec4 refPos;
	float diffDepth_old = texture(depthTexture, pos).x;
	vec3 normal_old = texture(normalTexture, pos).xyz;
	vec3 eye_old = texture(viewDirTexture, pos).xyz;
	
	passPosition = pos;
	warpedNormal =  projection * altView * vec4(normal_old,0);
	newViewDir =  (projection * altView * vec4(eye_old,0)).xyz;

	if(warpOnOff==0){
		if(diffDepth_old>=999){
			
			diffPos_old =   invViewProjection * vec4(pos * 2 - 1, 0.999, 1);
			warpedDiffPos = projection * altView * diffPos_old;
			gl_Position = projection * altView * diffPos_old;

			diffColor = texture(colorTexture,pos) + texture(indirectColorTexture, pos);
			
		}
		else {
			diffPos_old	= texture(diffPositionTexture, pos);
			vec3 viewDir_old = diffPos_old.xyz - eye_old;
			
			//Reconstruct position of first indirection
			vec4 refRay = normalize( reflect(vec4(viewDir_old,0) , texture(normalTexture, pos)));
			float refDepth = texture(depth2Texture, pos).x;
			refPos =  diffPos_old + refRay + refDepth;  //100% correct?
			
			
			warpedDiffPos = projection  * altView *  diffPos_old;
			gl_Position = projection  * altView *  diffPos_old ;
			
			diffColor = diffColor = texture(colorTexture,pos);
			
			
			//------------------//
			//warping reflection//
			//------------------//
			
			mat4 mvpNew = projection * altView;
			//layout (local_size_x = 16, local_size_y = 16) in; 
			//ivec2 inCoord = ivec2(gl_GlobalInvocationID.xy);  // used in compute shader
			
			vec2 inCoord = pos;
			vec3 wDifPos = texture(diffPositionTexture, inCoord).xyz;  // = diffPos_old
			vec3 wNorm = texture(normalTexture, inCoord).xyz;
			// vec3 wRefPos = texture(refPos, inCoord).xyz;  // rekonstruiert
			reflectionPosition = vec4(wDifPos - reflect(wDifPos - refPos.xyz, normalize(wNorm)), 1);
		
			//took iPos instead of reflectionPosition - correct?
			iPos = mvpNew * vec4(reflectionPosition.xyz,1);   //
			iPos /= iPos.w;
			coordColor = vec2(
			
			float(float(inCoord.x) / 1280.0),
			float(float(inCoord.y) / 720.0));
			
			//float(float(inCoord.x) / " + w + ".0),
			//float(float(inCoord.y) / " + h + ".0));
			
			ivec2 outCoord = ivec2(
			float(iPos.x + 1.0) * 0.5 * 720.0 + (1280.0-720.0)/2.0,
			float(iPos.y + 1.0) * 0.5 * 720.0);
			
			//	float(iPos.x + 1.0) * 0.5 * " + h + ".0 + (" + w + ".0-" + h + ".0)/2.0,
			//	float(iPos.y + 1.0) * 0.5 * " + h + ".0);
			
			vec4 lastIteration;
			ivec2 tmp;
			tmp = outCoord;
			//   lastIteration = texture(outTexture, tmp);
			lastIteration = texture(indirectColorTexture, tmp);
			if (lastIteration.z > iPos.z) {
			//	imageStore(texout, tmp, vec4(coordColor, iPos.z, 1.0));
				//outTexture = texture();
			//outCoo = vec4(outCoord, iPos.z, 1.0);
			}
		}	
	}
	
	else{
		gl_Position = vec4(pos * 2 - 1, 0, 1);
		//diffColor = warpedNormal ;

	}
}