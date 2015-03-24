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
    vec4 wPos = vec4(wDifPos - normalize(reflect(wDifPos - wRefPos,normalize(wNorm))) * distance(wDifPos, wRefPos), 1);

	gl_Position = (projection * altView * wPos) * vec4(1,1,0.999,1); //vec4(pos * 2 - 1, 0, 1);
	// gl_Position = vec4(pos,0,1); //vec4(pos * 2 - 1, 0, 1);
	
	// coord color
	coordColor = vec2(
    float(float(pos.x) /resolution.x),
    float(float(pos.y) / resolution.y));

}