#version 430

in vec2 pos;

uniform sampler2D reflectionPositionTexture; 
uniform sampler2D diffusePositionTexture;  
uniform sampler2D normalTexture;
uniform mat4 view;
uniform mat4 projection;

out vec2 passPosition;

void main() {
	passPosition = pos;
	
	// compute warped reflective position
	vec3 wDifPos = texture(diffusePositionTexture, pos).xyz;
    vec3 wNorm = texture(normalTexture, pos).xyz;
    vec3 wRefPos = texture(reflectionPositionTexture, pos).xyz;  
    vec4 wPos = vec4(wDifPos - normalize(reflect(wDifPos - wRefPos,normalize(wNorm))) * distance(wDifPos, wRefPos), 1);

	gl_Position = (projection * view * wPos) * vec4(1,1,0.999,1); //vec4(pos * 2 - 1, 0, 1);
	
	// coord color
	//coordColor = vec2(
    //float(float(pos.x) /resolution.x),
    //float(float(pos.y) / resolution.y));

}