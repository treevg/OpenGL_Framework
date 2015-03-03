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
//uniform sampler2D 	initDirTexture;
uniform sampler2D 	depth2Texture;

//uniform sampler2D 	reflectedPositionTexture;

uniform int 		texSwitch;
//uniform vec3 		initialPos;

layout ( rgba16f, binding = 0 ) uniform image2D 	texout;

in vec2 pos;

out vec2 passPosition;
out vec4 diffColor;  
out vec4 warpedNormal;
out vec4 warpedDiffPos;
out vec2 coordColor;
out vec4 reflectionPosition;

//out vec4 newEyeDir;

//out vec4 outTexture;
//out vec3 passColor;
//out vec4 passIndColor;



void main() {
	vec4 diffPos_old;
	float diffDepth_old = texture(depthTexture, pos).x;
	vec3 normal_old = texture(normalTexture, pos).xyz;
	//vec3 eye_old = texture(initDirTexture, pos).xyz;
	
	passPosition = pos;
	warpedNormal =  projection * altView * vec4(normal_old,0);
	//newEyeDir =  projection * altView * vec4(eye_old,0);

	//vec4 passColor	= texture(color,pos);
	//vec4 passIndColor 	= texture(indirectColorTexture, pos);

	if(warpOnOff==0){
		if(diffDepth_old>=999){
			
			diffPos_old =   invViewProjection * vec4(pos * 2 - 1, 0.999, 1);
			warpedDiffPos = projection * altView * diffPos_old;
			gl_Position = projection * altView * diffPos_old;

			diffColor = texture(colorTexture,pos) + texture(indirectColorTexture, pos);
			
		}
		else {
			
			//Reconstruct position of first indirection
			vec4 refRay = normalize( reflect(diffPos_old , texture(normalTexture, pos)));
			float refDepth = texture(depth2Texture, pos).x;
			vec4 refPos =  diffPos_old + refRay + refDepth;  //100% correct?
			
			
			diffPos_old	= texture(diffPositionTexture, pos);
			warpedDiffPos = projection  * altView *  diffPos_old;
			gl_Position = projection  * altView *  diffPos_old ;
			
			
			// warping reflection//
			
			mat4 mvpNew = projection * altView;
			//layout (local_size_x = 16, local_size_y = 16) in; 
			//ivec2 inCoord = ivec2(gl_GlobalInvocationID.xy);  // used in compute shader
			
			vec2 inCoord = pos;
			vec3 wDifPos = texture(diffPositionTexture, inCoord).xyz;
			vec3 wNorm = texture(normalTexture, inCoord).xyz;
			// vec3 wRefPos = texture(refPos, inCoord).xyz;  // rekonstruiert
			reflectionPosition = vec4(wDifPos - reflect(wDifPos - refPos.xyz, normalize(wNorm)), 1);
			vec4 iPos = mvpNew * vec4(reflectionPosition.xyz,1);   //
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
				imageStore(texout, tmp, vec4(coordColor, iPos.z, 1.0));
				//outTexture = texture();
			}


			if(texSwitch==1){
			diffColor = refPos;
			}
			else{
			diffColor = texture(colorTexture,pos);// + texture(indirectColorTexture, pos);	
			//diffColor = texture(reflectedPositionTexture, pos);
			}
		}	
	}
	
	else{
		gl_Position = vec4(pos * 2 - 1, 0, 1);
		//diffColor = texture(texout, pos) ;
		diffColor =texture(normalTexture, pos) ;// + (texture(indirectColorTexture,pos)-0.2);

	}
}