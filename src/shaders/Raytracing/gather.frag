#version 430

uniform sampler2D uvDiffuse;
uniform sampler2D uvReflect;
uniform sampler2D colorReflect;

uniform vec2 resolution; 

out vec4 warpedColor;




// vec3 reflectionVector = normalize(reflect(warpedDiffusePosition - eyePosition, normalize(warpedNormal.xyz)));

float reflectionQuality(vec2 coord) {
//     vec3 testReflectionPosition = texture(reflectionPositionTexture, coord).xyz;
//     vec3 testReflectionVector = normalize(testReflectionPosition - warpedDiffusePosition);
    // return acos(clamp(dot(
    //                       reflectionVector,
    //                       testReflectionVector), -1, 1));

	return 0;
}





vec2 interpolateGuess(vec2 g1, vec2 g2) {
    float g1Q = reflectionQuality(g1);
    float g2Q = reflectionQuality(g2);
    float q = g1Q + g2Q;
    return g1 * g2Q / q + g2 * g1Q / q;
}

void main() {
	vec2 uv = gl_FragCoord.xy / resolution;
	vec2 uvDiff = texture(uvDiffuse, uv).xy;
	vec2 uvRef = texture(uvReflect, uv).xy;

	if (length(uvRef) != 0) {
		vec2 uvNew = interpolateGuess(uvDiff, uvRef);
		warpedColor = texture(colorReflect, uvNew);
	} else {
		warpedColor = vec4(0,0,0,0);
	}
}