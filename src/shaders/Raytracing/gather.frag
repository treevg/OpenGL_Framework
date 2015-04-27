#version 430

uniform sampler2D uvDiffuse;
uniform sampler2D uvReflect;
uniform sampler2D colorReflect;
uniform sampler2D warpedDiffusePositionTexture;
uniform sampler2D warpedNormalTexture;
uniform sampler2D reflectionPositionTexture;
uniform vec2 resolution; 
uniform mat4 view;

uniform int test;
uniform int swap;
out vec4 warpedColor;

vec3 eyePosition = (inverse(view) * vec4(0,0,0,1)).xyz;
vec2 uv = gl_FragCoord.xy / resolution;
vec3 warpedDiffusePosition = texture(warpedDiffusePositionTexture, uv).xyz;
vec4 warpedNormal = texture(warpedNormalTexture,uv);
vec3 reflectionVector = normalize(reflect(warpedDiffusePosition - eyePosition, normalize(warpedNormal.xyz)));


//original
float reflectionQuality(vec2 coord) {
    vec3 testReflectionPosition = texture(reflectionPositionTexture, coord).xyz;
    vec3 testReflectionVector = normalize(testReflectionPosition - warpedDiffusePosition);
    return acos(clamp(dot(reflectionVector, testReflectionVector), -1, 1));
}

float reflectionQuality3(vec2 coord) {
    vec3 testReflectionPosition = texture(reflectionPositionTexture, coord).xyz;
    vec3 testReflectionVector = normalize(testReflectionPosition - warpedDiffusePosition);
	float angle = (dot(reflectionVector, testReflectionVector)) / (length(reflectionVector) * length(testReflectionVector));
	if(length(testReflectionVector)>=10) {
	return 0.0;
	}
	else {
	//return angle;
	return acos(angle) /2*3.1415;
  // return acos(clamp(dot(reflectionVector, testReflectionVector), -1, 1));
  }
}

float reflectionQuality2(vec2 coord) {
	if(length(coord)==0.0) {
	return 0.0;
	}
    vec3 testReflectionPosition = texture(reflectionPositionTexture, coord).xyz;
    vec3 testReflectionVector = normalize(testReflectionPosition - warpedDiffusePosition);
	float angle = (dot(reflectionVector, testReflectionVector)) / (length(reflectionVector) * length(testReflectionVector));

	return 1.0-angle;
	//return acos(angle) /2*3.1415;
}

float reflectionQualityTester(vec2 coord) {
    vec3 testReflectionPosition = texture(reflectionPositionTexture, coord).xyz;
    vec3 testReflectionVector = normalize(testReflectionPosition - warpedDiffusePosition);
    return dot(reflectionVector, testReflectionVector);
}

//TODO: avoid "black" coords
//wenn UV schwarz(==undefiniert), dann wird unten links(0,0) von refPosTex geguckt. =>falscher vektor entsteht...
//hierarchie der UVTex aufbauen? wenn an beiden Pos schwarz, dann def. undefinierte stelle
//idee: viewmatrix einbeziehen und als schätzung für guess benutzen. 
//idee: entfernung zum ursprung einbeziehen
//idee: komplexere gewichtung statt interpolation
//idee: mit "uv" direkt in refPosTex nachgucken


// avoiding "black" uvs with original uv
// problem: two interpoaltions...
vec2 reflectionQualityVec_try2(vec2 coordDiff, vec2 coordRef) {
	
	float chanceRefCoord = 0.5;
	float chanceDiffCoord = 0.5;

	if(length(coordDiff) == 0){
		chanceRefCoord = 0.6;
		chanceDiffCoord = 0.0;
	}
	else if(length(coordRef) == 0){
		chanceRefCoord = 0.0;
		chanceDiffCoord = 0.7;
	}
    vec3 testReflectionPosition_diff = texture(reflectionPositionTexture, coordDiff).xyz;
	vec3 testReflectionPosition_ref = texture(reflectionPositionTexture, coordRef).xyz;
	vec3 testReflectionPosition_orig = texture(reflectionPositionTexture, uv).xyz;
	
    vec3 testReflectionVector_diff = normalize(testReflectionPosition_diff - warpedDiffusePosition);
	vec3 testReflectionVector_ref = normalize(testReflectionPosition_ref - warpedDiffusePosition);
	vec3 testReflectionVector_orig = normalize(testReflectionPosition_orig - warpedDiffusePosition);
	
	float angleDiff = acos(clamp(dot(reflectionVector, testReflectionVector_diff), -1, 1));
	float angleRef = acos(clamp(dot(reflectionVector, testReflectionVector_ref), -1, 1));
	float angleOrig = acos(clamp(dot(reflectionVector, testReflectionVector_orig), -1, 1));

	float interpolateSum = angleDiff + angleRef;
	vec2 interpolateUV = coordDiff * angleRef * chanceDiffCoord/interpolateSum + 
						  coordRef * angleDiff * chanceRefCoord/interpolateSum;		
						  
	vec3 testReflectionPosition_inter = texture(reflectionPositionTexture, interpolateUV).xyz;
	vec3 testReflectionVector_inter = normalize(testReflectionPosition_inter - warpedDiffusePosition);
	
	float angleInter = acos(clamp(dot(reflectionVector, testReflectionVector_inter), -1, 1));
	interpolateSum += angleOrig;
	
	// not perfect
	return interpolateUV * angleOrig/interpolateSum * chanceDiffCoord+chanceRefCoord + 
		   testReflectionPosition_orig.xy * (1-chanceDiffCoord-chanceRefCoord) * angleInter/interpolateSum;
}

vec2 interpolateGuess(vec2 g1, vec2 g2) {
	// if(test==0){
	// 	vec2 refQuality = reflectionQualityVec_try2(g1, g2);
	// 	float q = refQuality.x + refQuality.y;
	// 	return g1 * refQuality.y / q + g2 * refQuality.x / q;
	// }
	
	// else{
	float g2Q ;
	float g1Q;
		if(swap==0){
			g1Q = reflectionQuality(g1);
			g2Q = reflectionQuality(g2);	
		}	
		else{
			g1Q = reflectionQuality2(g1);
			g2Q = reflectionQuality2(g2);	
		}
		float q = g1Q + g2Q;
		return (g1 * g1Q + g2 * g2Q) / q;
	//	return (g1 * g2Q + g2 * g1Q) / q;
	// }
}

vec2 interpolateGuessPrimitively(vec2 g1, vec2 g2) {
	if(reflectionQuality(g1) > reflectionQuality(g2)){
		return g1;
	} else {
		return g2;
	}
}

vec4 qualityVisualization(vec2 g1, vec2 g2) {
	float g1Q = 3.1415- reflectionQuality2(g1);
	float g2Q = 3.1415 - reflectionQuality2(g2);
	return vec4(g1Q ,0, g2Q, 1);		
}

vec4 qualityVisualization2(vec2 g1) {
	float g1Q = reflectionQuality2(g1);
	return vec4(g1Q);		
}

vec2 gradientDescent(vec2 initialGuess) {
    vec2 gradient;
    float g00;
    float g01;
	int maxGDSteps = 10;
	float newQuality;
	vec2 eps = vec2(1, 1); // / resolution;
	float bestQuality = 999.0;
	float l = 0.00001;
	float ensuredQualityThreshold = 5;
	bool undefined = false;

    for (int i = 0; i < maxGDSteps; i++) {
        newQuality = reflectionQuality(initialGuess);

        g00 = reflectionQuality(initialGuess + vec2(eps.x, 0));
        g01 = reflectionQuality(initialGuess + vec2(0, eps.y));
        
        gradient = l * vec2(g00 - newQuality, g01 - newQuality) / (eps);
        initialGuess -= gradient;

        if (newQuality < bestQuality) {
            bestQuality = newQuality;
        }
    }
    if (bestQuality > ensuredQualityThreshold) {
        undefined = true;  
	}
    return initialGuess;
}

void main() {

	vec2 uvDiff = texture(uvDiffuse, uv).xy;
	vec2 uvRef = texture(uvReflect, uv).xy;


	vec2 uvNew = interpolateGuess(uvDiff, uvRef);
	uvNew = gradientDescent(uvNew);
	
	//warpedColor = texture(colorReflect, uvRef);  
	//vec2 uvNew = interpolateGuessPrimitively(uvDiff, uvRef);
	warpedColor = texture(colorReflect, uvNew);  

	if(test==0) {
		warpedColor = qualityVisualization2(uvRef);
	}
	

}