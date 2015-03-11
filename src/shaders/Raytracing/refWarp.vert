#version 430

in vec2 pos;

//uniform sampler2D outTexture;  

uniform sampler2D reflectionPositionTexture; 
uniform sampler2D diffusePositionTexture;  
uniform sampler2D normalTexture;			 
uniform mat4 altView;
uniform mat4 projection;

out vec2 passPosition;
out vec4 warpRefPos;

void main() {
	mat4 mvpNew = projection * altView;
	passPosition = pos;
	
	vec3 wDifPos = texture(diffusePositionTexture, pos).xyz;
    vec3 wNorm = texture(normalTexture, pos).xyz;
    vec3 wRefPos = texture(reflectionPositionTexture, pos).xyz;  
    vec4 wPos = vec4(wDifPos - reflect(wDifPos - wRefPos,normalize(wNorm)), 1);
    warpRefPos = mvpNew * vec4(wPos.xyz,1);
    warpRefPos /= warpRefPos.w;
	
	
	//hard-coded resolution
	vec2 outCoord = vec2(
    float(warpRefPos.x + 1.0) * 0.5 * 720.0 + (1280.0 - 720.0)/2.0,
    float(warpRefPos.y + 1.0) * 0.5 * 720.0);
	
	
//	gl_Position = vec4(pos, 0, 1);
	gl_Position = vec4(outCoord, warpRefPos.z, 1);
	gl_Position = warpRefPos;
	
	
}