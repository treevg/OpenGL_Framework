#version 430

in vec2 pos;

//uniform sampler2D outTexture;  

uniform sampler2D reflectionPositionTexture; 
uniform sampler2D diffusePositionTexture;  
uniform sampler2D normalTexture;
uniform mat4 altView;
uniform mat4 projection;
uniform vec2 resolution;

out vec2 passPosition;
//out vec4 warpRefPos;
out vec2 coordColor;

void main() {
	mat4 mvpNew = projection * altView;
	passPosition = pos;
	
	// compute warped reflective position
	vec3 wDifPos = texture(diffusePositionTexture, pos).xyz;
    vec3 wNorm = texture(normalTexture, pos).xyz;
    vec3 wRefPos = texture(reflectionPositionTexture, pos).xyz;  
    vec4 wPos = vec4(wDifPos - reflect(wDifPos - wRefPos,normalize(wNorm)), 1);
	//vec4 warpRefPos =  projection * altView * vec4(wPos.xyz,1);
	//warpRefPos /= warpRefPos.w;
	//warpRefPos = projection * altView * wPos;
	//gl_Position = warpRefPos;

	//'infinite' depth
	gl_Position = (projection * altView * wPos) * vec4(1,1,0.001,1); //vec4(pos * 2 - 1, 0, 1);
	// gl_Position = (projection * altView * wPos) * vec4(1,1,-1,1); //vec4(pos * 2 - 1, 0, 1);
	

	
	// coord color
	coordColor = vec2(
    float(float(pos.x) /resolution.x),
    float(float(pos.y) / resolution.y));
	
//	vec2 outCoord = vec2(
//    float(warpRefPos.x + 1.0) * 0.5 * resolution.y + (resolution.x - resolution.y)/2.0,
//    float(warpRefPos.y + 1.0) * 0.5 * resolution.y);

	
	//leftout - needed at all?
	//vec4 lastIteration;
    //lastIteration = texture(outTexture, outCoord);
    //if (lastIteration.z > iPos.z) {
    //    imageStore(outTexture, outCoord, vec4(coordColor, iPos.z, 1.0));
    //}
	
	


	
	
}