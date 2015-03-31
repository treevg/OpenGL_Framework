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

uniform int test;
out vec4 warpedColor;

vec3 eyePosition = (inverse(view) * vec4(0,0,0,1)).xyz;
vec2 uv = gl_FragCoord.xy  / resolution;
vec3 warpedDiffusePosition = texture(warpedDiffusePositionTexture, uv).xyz;
vec4 warpedNormal = texture(warpedNormalTexture,uv);
vec3 reflectionVector = normalize(reflect(warpedDiffusePosition - eyePosition, normalize(warpedNormal.xyz)));

vec4 g = vec4(0);


//original
float reflectionQuality(vec2 coord) {
    vec3 testReflectionPosition = texture(reflectionPositionTexture, coord).xyz;
    vec3 testReflectionVector = normalize(testReflectionPosition - warpedDiffusePosition);
    return acos(clamp(dot(reflectionVector, testReflectionVector), -1, 1));
}

//TODO: avoid "black" coords
//wenn UV schwarz(==undefiniert), dann wird in mitte(0,0) von refPosTex geguckt. =>falscher vektor entsteht...
//hierarchie der UVTex aufbauen? wenn an beiden Pos schwarz, dann def. undefinierte stelle
//idee: viewmatrix einbeziehen und als schätzung für guess benutzen. ->schwarze UV effizienter vermeiden
//idee: entfernung zum ursprung einbeziehen
//idee: komplexere gewichtung statt interpolation
//idee: mit "uv" direkt in refPosTex nachgucken

// searching in neighbourhood for non-black uvs
float reflectionQuality_diff(vec2 coord) {
	vec2 newCoord = coord;
	if(length(coord)== 0){
		vec2 newUV = uv;
		int i = 0;
		while(length(newCoord) == 0 && i<15){
			newUV += vec2(1/resolution.x, 0);
			newCoord = texture(uvDiffuse, newUV).xy;
			i++;
		}
	}
    vec3 testReflectionPosition = texture(reflectionPositionTexture, newCoord).xyz;
    vec3 testReflectionVector = normalize(testReflectionPosition - warpedDiffusePosition);
    return acos(clamp(dot(reflectionVector, testReflectionVector), -1, 1));
}
float reflectionQuality_ref(vec2 coord) {
	vec2 newCoord = coord;
	if(length(coord)== 0){
		vec2 newUV = uv;
		int i = 0;
		while(length(newCoord) == 0 && i<15){
			newUV += vec2(1/resolution.x, 0);
			newCoord = texture(uvReflect, newUV).xy;
			i++;
		}
	}
    vec3 testReflectionPosition = texture(reflectionPositionTexture, newCoord).xyz;
    vec3 testReflectionVector = normalize(testReflectionPosition - warpedDiffusePosition);
    return acos(clamp(dot(reflectionVector, testReflectionVector), -1, 1));
}

// avoiding "black" uvs with original uv
// problem: two interpoaltions...
vec2 reflectionQualityVec_try(vec2 coordDiff, vec2 coordRef) {
	float chanceRefCoord = 0.5;
	float chanceDiffCoord = 0.5;

	if(length(coordDiff) == 0){
		chanceRefCoord = 0.8;
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
	vec2 interpolateUV = (coordDiff*angleRef*chanceDiffCoord/interpolateSum + coordRef*angleDiff*chanceRefCoord/interpolateSum);				 
	vec3 testReflectionPosition_inter = texture(reflectionPositionTexture, interpolateUV).xyz;
	vec3 testReflectionVector_inter = normalize(testReflectionPosition_inter - warpedDiffusePosition);
	float angleInter = acos(clamp(dot(reflectionVector, testReflectionVector_inter), -1, 1));
	float interpolateSum_2 = interpolateSum + angleOrig;
	
	return interpolateUV*angleOrig/interpolateSum_2*chanceDiffCoord+chanceRefCoord + uv*(1-chanceDiffCoord-chanceRefCoord)*angleInter/interpolateSum_2;
}

// more compact version 
vec2 reflectionQualityVec(vec2 coordDiff, vec2 coordRef) {
    vec3 testReflectionPosition_diff = texture(reflectionPositionTexture, coordDiff).xyz;
	vec3 testReflectionPosition_ref = texture(reflectionPositionTexture, coordRef).xyz;	
    vec3 testReflectionVector_diff = normalize(testReflectionPosition_diff - warpedDiffusePosition);
	vec3 testReflectionVector_ref = normalize(testReflectionPosition_ref - warpedDiffusePosition);

    return vec2(acos(clamp(dot(reflectionVector, testReflectionVector_diff), -1, 1)),
				acos(clamp(dot(reflectionVector, testReflectionVector_ref), -1, 1)));
				
}

vec2 interpolateGuess(vec2 g1, vec2 g2) {
	if(test==1){
	vec2 refQuality = reflectionQualityVec_try(g1, g2);
	float q = refQuality.x + refQuality.y;
	return g1 * refQuality.y / q + g2 * refQuality.x / q;

	
//	float g1Q = reflectionQuality_diff(g1);
//	float g2Q = reflectionQuality_ref(g2);		
//	float q = g1Q + g2Q;
//	return g1 * g2Q / q + g2 * g1Q / q;
	}
	
	else{
	vec2 refQuality = reflectionQualityVec(g1, g2);
	float q = refQuality.x + refQuality.y;
	return g1 * refQuality.y / q + g2 * refQuality.x / q;
	}
}

vec2 gradientDescent(vec2 initialGuess) {
    vec2 gradient;
    float g00;
    float g01;
	int maxGDSteps = 5;
	float newQuality;
	// pixel vs. subpixel
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
	vec2 uvVar = texture(temp, uv).xy;
	

	if (true) {
	//	if (length(uvRef) != 0) {
		vec2 uvNew = interpolateGuess(uvDiff, uvRef);
		uvNew = gradientDescent(uvNew);
		warpedColor = texture(colorReflect, uvNew);  
		} 
		else {
		warpedColor = vec4(0,0,0,0);
	}
	//warpedColor = g;
}