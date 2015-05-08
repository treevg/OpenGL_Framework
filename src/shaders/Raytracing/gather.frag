#version 430

uniform sampler2D uvDiffuse;
uniform sampler2D uvReflect;
uniform sampler2D colorReflect;
uniform sampler2D warpedDiffusePositionTexture;
uniform sampler2D warpedNormalTexture;
uniform sampler2D oldReflectionPositionTexture;
uniform vec2 resolution; 
uniform int gradientDescentSteps; 
uniform mat4 view;

out vec4 warpedColor;

vec3 eyePosition = (inverse(view) * vec4(0,0,0,1)).xyz;
vec2 uv = gl_FragCoord.xy / resolution;
vec3 warpedDiffusePosition = texture(warpedDiffusePositionTexture, uv).xyz;
vec4 warpedNormal = texture(warpedNormalTexture,uv);
vec3 reflectionVector = normalize(reflect(warpedDiffusePosition - eyePosition, normalize(warpedNormal.xyz)));


//original
float reflectionQuality(vec2 coord) {
	vec3 r = normalize(vec4(reflectionVector,1).xyz);
    vec3 testReflectionPosition = texture(oldReflectionPositionTexture, coord).xyz;
    vec3 testReflectionVector = normalize(testReflectionPosition - warpedDiffusePosition);
    return acos(clamp(dot(r, testReflectionVector), -1, 1));
}

vec2 gradientDescent(vec2 initialGuess) {
    vec2 gradient;
    float g00;
    float g01;
	int maxGDSteps = gradientDescentSteps;
	float newQuality;
	// pixel vs. subpixel
	vec2 eps = vec2(1, 1) / resolution;
	float bestQuality = 999.0;
	float l = 0.00001;
	float ensuredQualityThreshold = 0.03;
	bool undefined = false;

	vec2 outputCoord = initialGuess;

    for (int i = 0; i < maxGDSteps; i++) {
        newQuality = reflectionQuality(outputCoord);

        g00 = reflectionQuality(outputCoord + vec2(eps.x, 0));
        g01 = reflectionQuality(outputCoord + vec2(0, eps.y));
        
        gradient = l * vec2(g00 - newQuality, g01 - newQuality) / (eps);
        outputCoord -= gradient;
		
			//dynamic steps
		//l /= i/10.0;
		// l *= clamp(mod(i,25),1.0, 0.00001);

        if (newQuality < bestQuality) {
            bestQuality = newQuality;
        }
			//performance increase
		if(bestQuality <= ensuredQualityThreshold) {
			return outputCoord;}
    }
    if (bestQuality > ensuredQualityThreshold) {
        undefined = true; 
        discard;
	}
    return outputCoord;
}

void main() {

	vec2 uvDiff = texture(uvDiffuse, uv).xy;
	vec2 uvRef = texture(uvReflect, uv).xy;

	float g1Q = reflectionQuality(uvRef);
	float g2Q = reflectionQuality(uvDiff);

		//no black holes at the beginning   
	if (length(uvRef) < 0.1 || g1Q == g2Q) {
		g1Q = 1;
		g2Q = 0;
	}
	
	vec2 initialGuess = (uvRef * g2Q + uvDiff * g1Q) / (g1Q + g2Q);
	vec2 coord = gradientDescent(initialGuess);
	warpedColor = texture(colorReflect, coord);  
}