#version 430

uniform sampler2D uvDiffuse;
uniform sampler2D uvReflect;
uniform sampler2D colorReflect;
uniform sampler2D warpedDiffusePositionTexture;
uniform sampler2D warpedNormalTexture;
uniform sampler2D reflectionPositionTexture;
uniform sampler2D diffFlow;
uniform vec2 resolution; 
uniform mat4 view;

uniform int test;
uniform int swap;
out vec4 warpedColor;

vec3 eyePosition = (inverse(view) * vec4(0,0,0,1)).xyz;
vec2 uv = gl_FragCoord.xy / resolution;
vec3 warpedDiffusePosition = texture(warpedDiffusePositionTexture, uv).xyz;
vec3 warpedNormal = normalize(texture(warpedNormalTexture,uv).xyz);
vec3 viewVector = normalize(warpedDiffusePosition - eyePosition);
vec3 invViewVector = normalize(eyePosition - warpedDiffusePosition);
vec3 reflectionVector = normalize(reflect(viewVector, normalize(warpedNormal)));

vec3 testReflectionVector = vec3(0);
vec4 temp = vec4(0);

//original
float reflectionQuality(vec2 coord) {
    vec3 testReflectionPosition = texture(reflectionPositionTexture, coord).xyz;
    vec3 testReflectionVector = normalize(testReflectionPosition - warpedDiffusePosition);
    return acos(clamp(dot(reflectionVector, testReflectionVector), -1, 1));
}

vec2 interpolateAnd(vec2 uvDiff, vec2 uvRef){
    vec3 testReflectionPositionDiff = texture(reflectionPositionTexture, uvDiff).xyz;
	vec3 testReflectionVectorDiff = testReflectionPositionDiff - warpedDiffusePosition;
	vec3 testReflectionPositionRef = texture(reflectionPositionTexture, uvRef).xyz;
	vec3 testReflectionVectorRef = testReflectionPositionRef - warpedDiffusePosition;
	
	if(length(testReflectionVectorDiff)>5) {
	return uvDiff;
	}
	else {
	return uvRef;
	}

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

vec3 optNormal() {
	//temp = vec4(0,0,1,0);
	//testReflectionVector = (viewVector + warpedNormal) / 4 ;
	//testReflectionVector =  warpedNormal ;
	return testReflectionVector;
}

bool flatNormal() {
	bool isFlat = false;
	vec3 wnR = (texture(warpedNormalTexture,uv+vec2(10.0/resolution.x,0.0)).xyz);
	vec3 wnL = (texture(warpedNormalTexture,uv+vec2(-10.0/resolution.x,0.0)).xyz);
	//vec3 wnU = normalize(texture(warpedNormalTexture,uv+vec2(0.0,2.0)).xyz);
	//vec3 wnD = normalize(texture(warpedNormalTexture,uv+vec2(0.0,-2.0)).xyz);

	if(dot(wnR, wnL)-1 >-0.0005 && dot(wnR, wnL)-1 <0.0005){
	//temp = vec4(0,0,1,0);
	isFlat=true;
	}
	return isFlat;
}
float reflectionQuality2(vec2 coord) {

/**  return = 
* 	dot() almost 1 -> planar reflection
*	distance() > 5 -> diff reflection   far reflection
*	length(coord) == 0 -> diff reflection
*
*
*
**/
	
	// test for "black" coords
	if(length(coord)==0.0) {return 4.0;}

    vec3 testReflectionPosition = texture(reflectionPositionTexture, coord).xyz;
	testReflectionVector = normalize(testReflectionPosition - warpedDiffusePosition);
	
	// TODO: bend normals/ reflectionVec towards camera
	if(dot( invViewVector, warpedNormal)<0.2) {
		optNormal();
	}
	
	//float angle = acos(abs(dot( reflectionVector, testReflectionVector) / (length(reflectionVector) * length(testReflectionVector))));
	float angle = (dot( reflectionVector, testReflectionVector) +1) /2;

	if(testReflectionPosition.y>0.9 ) {
		//temp = vec4(0,0,1,0);
		//return 1.0;
	}
	
	if(dot( reflectionVector, testReflectionVector)<0.1) {
		//return 2.0;
		//temp = vec4(0,0,1,0);
	}
	
	if(dot( reflectionVector, testReflectionVector)<0.1) {
		//return 2.0;
		//temp = vec4(0,0,1,0);
	}
	if(dot( reflectionVector, testReflectionVector)>0.96) {
		//temp = vec4(1,0,0,0);
		return 0.0;
	}
	
	if(distance(warpedDiffusePosition, testReflectionPosition)>5.0 && length(warpedDiffusePosition)==0.0) {
		//temp = vec4(0,0,1,0);
		//return 3.0;
	}
	if(distance(warpedDiffusePosition, testReflectionPosition)>5.0) {
		//temp = vec4(0,0,1,0);
		return 3.0;
	}
	
	//else{return 1.0;}
	//return angle/2*3.1415;
	
	return angle;
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


vec2 interpolateGuess2(vec2 g1, vec2 g2) {
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

vec2 interpolateGuess(vec2 g1, vec2 g2) {
	float g2Q;
	float g1Q;
	float saveQ1;
	float saveQ2;
	bool axel = false;
	
	g1Q = reflectionQuality2(g1); //Diff
	g2Q = reflectionQuality2(g2); //Ref
	saveQ1 = g1Q;
	saveQ2 = g2Q;
	
	//&& length(g2)!= 0.0
	//if(g1Q - g2Q != 0.0 && g2Q!= 0.0) {
	
	
	
	if(g1Q==3.0) {
		
		g1Q = 0.0;
		g2Q = 1.0;
		float q = g1Q + g2Q;
		return (g2 * g1Q + g1 * g2Q) / q; 	
	}
	
	if(g2Q == 0.0) {
		g1Q = 0.0;
		g2Q = 1.0;
		
	}
	else if(g2Q == 4.0) {
		g1Q = 1.0;
		g2Q = 0.0;
	}
	else {
		
		if(flatNormal()) {
			g1Q = 0.0;
			g2Q = 1.0;
			temp = vec4(1,0,0,0);
		}
		else {
			g1Q = 1.0;
			g2Q = 0.0;
		}
	}
	
	float q = g1Q + g2Q;
	return (g1 * g1Q + g2 * g2Q) / q; 
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
//	float g1Q = reflectionQualityTester(g1);
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
	vec2 flow = texture(diffFlow, uv).xy;

	//vec2 uvGuess = interpolateGuessPrimitively(uvDiff, uvRef);
	//vec2 uvGuess = interpolateGuess(uvDiff, uv/resolution + uvRef);
	vec2 uvGuess = interpolateGuess(uvDiff, uvRef);

	//vec2 uvGuess = interpolateAnd(uvDiff, uvRef);

	
	//uvGuess = gradientDescent(uvGuess);
	warpedColor = texture(colorReflect, uvGuess);  

	warpedColor+=temp;

	if(test==0) {
		warpedColor = qualityVisualization2(uvRef);
	}
	

}