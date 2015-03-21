#version 430
in vec4 passPosition;

layout(location = 0) out vec4 warpedColor;

uniform sampler2D reflectionColorTexture;   	
uniform sampler2D reflectionPositionTexture;    
uniform sampler2D warpedDiffusePositionTexture;
uniform sampler2D splattedReflectionUVTexture;
uniform sampler2D warpedNormalTexture;
uniform vec2 resolution;
uniform mat4 mvpOld;
uniform int mode;
uniform int maxGDSteps;
uniform mat4 altView;
//uniform sampler2D diffColorTexture;

float ratio = resolution.x / resolution.y;
bool undefined = false;
vec4 invalidColor = vec4(0, 0, 0, 0);

vec2 splattedReflectionUV = texture(splattedReflectionUVTexture, (passPosition.xy -1) * 0.5).xy;
vec3 warpedDiffusePosition = texture(warpedDiffusePositionTexture, (passPosition.xy -1) * 0.5).xyz;
vec4 warpedNormal = texture(warpedNormalTexture,(passPosition.xy -1) * 0.5);

//troublemaker - correct now?
vec3 eyeNew = vec4(inverse(altView) * vec4(0,0,0,1)).xyz;
vec3 reflectionVector = normalize(reflect(warpedDiffusePosition - eyeNew, normalize(warpedNormal.xyz)));

vec2 initialCoord() {
    vec3 closestCoord = vec3(0,0,9999);

    vec3 lookup;
    lookup = texelFetch(splattedReflectionUVTexture, ivec2(gl_FragCoord.xy + vec2(0, 0)), 0).xyz;
    if (lookup.z < closestCoord.z) {
        closestCoord = lookup;
    }
    lookup = texelFetch(splattedReflectionUVTexture, ivec2(gl_FragCoord.xy + vec2(0, 1)), 0).xyz;
    if (lookup.z < closestCoord.z) {
        closestCoord = lookup;
    }
    lookup = texelFetch(splattedReflectionUVTexture, ivec2(gl_FragCoord.xy + vec2(1, 0)), 0).xyz;
    if (lookup.z < closestCoord.z) {
        closestCoord = lookup;
    }
    lookup = texelFetch(splattedReflectionUVTexture, ivec2(gl_FragCoord.xy + vec2(0, -1)), 0).xyz;
    if (lookup.z < closestCoord.z) {
        closestCoord = lookup;
    }
    lookup = texelFetch(splattedReflectionUVTexture, ivec2(gl_FragCoord.xy + vec2(-1, 0)), 0).xyz;
    if (lookup.z < closestCoord.z) {
        closestCoord = lookup;
    }
    return closestCoord.xy  + 1 / resolution;
}


vec2 curvedGuess() {
    vec4 newCoordView = mvpOld * vec4(warpedDiffusePosition.xyz, 1);
    return vec2(
               (newCoordView.x / newCoordView.w / ratio + 1) * 0.5,
               (newCoordView.y / newCoordView.w + 1) * 0.5);
}


float reflectionQuality(vec2 coord) {
    vec3 testReflectionPosition = texture(reflectionPositionTexture, coord).xyz;
    vec3 testReflectionVector = normalize(testReflectionPosition - warpedDiffusePosition);
    return acos(clamp(dot(
                          reflectionVector,
                          testReflectionVector), -1, 1));
}


vec2 eps = vec2(1, 1) / resolution;
float l = 0.00001;
vec2 bestICoord;
float ensuredQualityThreshold = 5;  //old value= 0.04
float bestQuality = 999;
float newQuality;

vec2 gradientDescent(vec2 initialGuess) {
    vec2 iCoord = initialGuess;
    vec2 gradient;
    float g00;
    float g01;

    for (int i = 0; i < maxGDSteps; i++) {
        newQuality = reflectionQuality(iCoord);

        g00 = reflectionQuality(iCoord + vec2(eps.x, 0));
        g01 = reflectionQuality(iCoord + vec2(0, eps.y));
        
        gradient = l * vec2(g00 - newQuality, g01 - newQuality) / (eps);
        iCoord -= gradient;

        if (newQuality < bestQuality) {
            bestQuality = newQuality;
            bestICoord = iCoord;
        }
    }

    if (bestQuality > ensuredQualityThreshold) {
        undefined = true; 
    }
    return iCoord;
}

float g2Q;

vec2 interpolateGuess(vec2 g1, vec2 g2) {
    float g1Q = reflectionQuality(g1);
    g2Q = reflectionQuality(g2);
    float q = g1Q + g2Q;
    return g1 * g2Q / q + g2 * g1Q / q;
}

void main() {
    vec2 coord;
	
    // if (mode == 0) {
        vec2 splat = initialCoord();
        vec2 curvedGuess = curvedGuess();
        vec2 guess = interpolateGuess(splat, curvedGuess);
		
        if(splat.x > 999) {
           discard;
        } 

        coord = gradientDescent(guess);

        if (undefined || isnan(coord.x)) {
            warpedColor = invalidColor;
			//warpedColor = texture(diffColorTexture, (passPosition.xy - 1) * 0.5);

        } else {
            warpedColor = vec4(texture(reflectionColorTexture, coord + vec2(0.5,0) / resolution).xyz,1);
	   }

        if (warpedColor.x == 1) {
            warpedColor = vec4(0,0,0,0);
        }
    // }

    if (mode == 1) {
    warpedColor = vec4(bestQuality);
	//warpedColor = texture(diffColorTexture, (passPosition.xy - 1) * 0.5);
    }
}