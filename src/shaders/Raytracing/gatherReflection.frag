#version 430

//in vec2 texCoord;

in vec4 passPosition;

layout(location = 0) out vec4 warpedColor;

uniform sampler2D reflectionColorTexture;   	// indirectColor aus raytrace2.frag ?
uniform sampler2D reflectionPositionTexture;    //iPos aus refWarp.vert
uniform sampler2D warpedDiffusePositionTexture;
uniform sampler2D splattedReflectionUVTexture;
uniform sampler2D warpedNormalTexture;
uniform vec2 resolution;
uniform mat4 mvpOld;
uniform int mode;
uniform int maxGDSteps;

uniform sampler2D diffColorTexture;
uniform sampler2D eyeNewDirTexture;  // compute with invViewProjection


//uniform sampler2D coord;


// uniform sampler2D diffusePositionTexture;
// uniform sampler2D normalTexture;
// uniform mat4 mvpNew;
// uniform vec3 eyeOld;

float ratio = resolution.x / resolution.y;

bool undefined = false;
vec4 invalidColor = vec4(0, 0, 0, 0);

//texCoord replaced by passPosition. 

		// vec2 splattedReflectionUV = texelFetch(splattedReflectionUVTexture, ivec2(gl_FragCoord.xy), 0).xy + 1/resolution;
vec2 splattedReflectionUV = texture(splattedReflectionUVTexture, (passPosition.xy -1) * 0.5).xy;
vec3 warpedDiffusePosition = texture(warpedDiffusePositionTexture, (passPosition.xy -1) * 0.5).xyz;
vec4 warpedNormal = texture(warpedNormalTexture,(passPosition.xy -1) * 0.5);
vec3 eyeNew = texture(eyeNewDirTexture, (passPosition.xy -1) * 0.5).xyz;
vec3 reflectionVector = normalize(reflect(warpedDiffusePosition - eyeNew, normalize(warpedNormal.xyz)));
		// vec4 reflectionPosition = texture(reflectionPositionTexture, (passPosition.xy -1) * 0.5);

//vec4 temp = texture( splattedReflectionUVTexture, (texture(coord, passPosition.xy)).xy);

vec2 initialCoord() {
    vec3 closestCoord = vec3(0,0,9999);

    // for (int i = -1; i <= 1; i++) {
    //     for (int j = -1; j <= 1; j++) {
    //         vec3 lookup = texelFetch(splattedReflectionUVTexture, ivec2(gl_FragCoord.xy + vec2(i, j)), 0).xyz;
    //         if (lookup.z < closestCoord.z) {
    //             closestCoord = lookup;
    //         }
    //     }
    // }
    // return closestCoord.xy  + 1/resolution;

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
    // if(testReflectionPosition == 0) {
    //     return -1;
    // }
    return acos(clamp(dot(
                          reflectionVector,
                          testReflectionVector), -1, 1));
}


vec2 eps = vec2(1, 1) / resolution;
float l = 0.00001;
vec2 bestICoord;
// float ensuredQualityThreshold = 0.04;
float ensuredQualityThreshold = 5;
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
        // g10 = reflectionQuality(iCoord + vec2(-eps.x, 0));
        // g11 = reflectionQuality(iCoord + vec2(0, -eps.y));
        
        gradient = l * vec2(g00 - newQuality, g01 - newQuality) / (eps);

        // if (length(gradient) > 0.01) {
        //     l *= 0.3;
        //     continue;
        // }

        iCoord -= gradient;

        if (newQuality < bestQuality) {
            bestQuality = newQuality;
            bestICoord = iCoord;
        }
    }

    if (bestQuality > ensuredQualityThreshold) {
        undefined = true; 
    }

    // return bestICoord;
    return iCoord;
}


// vec2 interpolateGuess(vec2 g1, vec2 g2) {
//     float g1Q = reflectionQuality(g1);
//     float g2Q = reflectionQuality(g2);
//     if (g1Q < g2Q) {
//         return g1;
//     } else {
//         return g2;
//     }
// }

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
            // guess = curvedGuess;
        } 

        // warpedColor = vec4(g2Q);
        // return;

        coord = gradientDescent(guess);

        // vec4(1,0,0,1);
        if (undefined || isnan(coord.x)) {
            //warpedColor = invalidColor;
			
			warpedColor = texture(diffColorTexture, (passPosition.xy - 1) * 0.5);

        } else {
            // vec4 w1 = vec4(texture(reflectionColorTexture, coord + vec2(0.5) / resolution).xyz,1); 
            // vec4 w2 = vec4(texture(reflectionColorTexture, coord + vec2(1.0) / resolution).xyz,1); 
            // vec4 w3 = vec4(texture(reflectionColorTexture, coord + vec2(0.0) / resolution).xyz,1); 
            // warpedColor = (w1+w2+w3)/3;
            warpedColor = vec4(texture(reflectionColorTexture, coord + vec2(0.5,0) / resolution).xyz,1);
	   }

        if (warpedColor.x == 1) {
            warpedColor = vec4(0,0,0,0);
        }
    // }

    if (mode == 1) {

        // warpedColor = vec4(1,0,0,1);
     
        warpedColor = vec4(bestQuality);
		warpedColor = texture(diffColorTexture, (passPosition.xy - 1) * 0.5);

    }
}