#version 430

uniform sampler2D uvDiffuse;
uniform sampler2D uvReflect;
uniform sampler2D colorReflect;
uniform sampler2D warpedDiffusePositionTexture;
uniform sampler2D warpedNormalTexture;
uniform sampler2D reflectionPositionTexture;
uniform sampler2D temp;

uniform vec2 resolution; 
uniform mat4 view;
out vec4 warpedColor;

vec3 eyePosition = (inverse(view) * vec4(0,0,0,1)).xyz;
vec2 uv = gl_FragCoord.xy / resolution;
vec3 warpedDiffusePosition = texture(warpedDiffusePositionTexture, uv).xyz;
vec4 warpedNormal = texture(warpedNormalTexture,uv);

vec3 reflectionVector = normalize(reflect(warpedDiffusePosition - eyePosition, normalize(warpedNormal.xyz)));

float reflectionQuality(vec2 coord) {
    vec3 testReflectionPosition = texture(reflectionPositionTexture, coord).xyz;
    vec3 testReflectionVector = normalize(testReflectionPosition - warpedDiffusePosition);
    return acos(clamp(dot(reflectionVector, testReflectionVector), -1, 1));
}

vec2 interpolateGuess(vec2 g1, vec2 g2) {
    float g1Q = reflectionQuality(g1);
    float g2Q = reflectionQuality(g2);
    float q = g1Q + g2Q;
    return g1 * g2Q / q + g2 * g1Q / q;
}

void main() {

	vec2 uvDiff = texture(uvDiffuse, uv).xy;
	vec2 uvRef = texture(uvReflect, uv).xy;
	vec2 uvVar = texture(temp, uv).xy;
	

	if (length(uvRef) != 0) {
		vec2 uvNew = interpolateGuess(uvDiff, uvRef);
		warpedColor = texture(colorReflect, uvVar);   //uvNew
	} else {
		warpedColor = vec4(0,0,0,0);
	}
}