#version 430 

// basics taken from: https://www.shadertoy.com/view/ldS3DW
// triangle intersection taken from: http://undernones.blogspot.de/2010/12/gpu-ray-tracing-with-glsl.html
// edited by moe11elf

in vec4 gl_FragCoord;

uniform vec3	iResolution; 	
uniform mat4	projection;
uniform float 	zoom;
uniform int		indirection;
uniform sampler2D environmentTexture;

uniform mat4 	invView;
uniform mat4 	view;
uniform mat4	invViewProjection;
uniform mat3	normalMat;

uniform vec4 	sphereVec[3];
uniform vec3 	mesh[6];
uniform vec3 	colorSphere[3];
uniform vec3 	colorTriangle[3];

in 		vec4	passPosition;

//direct 
out 	vec4	diffuseColor;
out 	vec4 	diffusePosition;
out 	vec3	diffuseDepth;
out 	vec3	normal;
out		vec3	initialDirNotnorm;


//indirect
out 	vec4	reflectiveColor;
out 	vec4 	reflectivePosition;
out 	vec4	reflectiveDepth;


layout(std430, binding=11) buffer meshData{
	vec4 pos[4200];
} myMesh;

layout(std430, binding=12) buffer meshNormal{
	vec4 posNorm[4200];
} myNormals;


vec3 	light = normalize(vec3(sin(0), 0.6, cos(0)));
vec3 	currentColor = vec3(1,1,1);
vec3 	currentColor2 = vec3(1,1,1);
float 	currentDepth;
vec3 	currentNormal;
vec3 	currentDirNotnorm;

float sphere(vec3 ray, vec3 dir, vec3 center, float radius)
{
	vec3 rc = ray-center;
	float c = dot(rc, rc) - (radius*radius);
	float b = dot(dir, rc);
	float d = b*b - c;
	float t = -b - sqrt(abs(d));
	float st = step(0.0, min(t,d));
	return mix(-1.0, t, st);
}

float triangle(vec3 orig, vec3 dir, vec3 vertex0, vec3 vertex1, vec3 vertex2)
{
    vec3 u, v, n; // triangle vectors
    vec3 w0, w;  // ray vectors
    float r, a, b; // params to calc ray-plane intersect

    // get triangle edge vectors and plane normal
    u = vertex1 - vertex0;
    v = vertex2 - vertex0;
    n = normalize( cross(u,v) );

    w0	= orig - vertex0;
    a	= -dot(n, w0);
    b	= dot(n, dir);
    if (abs(b) < 1e-5){
        // ray is parallel to triangle plane, and thus can never intersect.
        return -1.0;
    }

    // get intersect point of ray with triangle plane
    r = a / b;
    if (r < 0.0)
        return -1.0; // ray goes away from triangle.

    vec3 I = orig + r * dir;
    float uu, uv, vv, wu, wv, D;
    uu = dot(u, u);
    uv = dot(u, v);
    vv = dot(v, v);
    w = I - vertex0;
    wu = dot(w, u);
    wv = dot(w, v);
    D = uv * uv - uu * vv;

    // get and test parametric coords
    float s, t;
    s = (uv * wv - vv * wu) / D;
    if (s < 0.0 || s > 1.0)
        return -1.0;
    t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0)
        return -1.0;

    return (r > 1e-5) ? r : -1.0;
}

vec3 background(vec3 rd)
{
	//cubemapping
	return vec3(texture(environmentTexture, vec2(atan(rd.x, rd.z) / 6.2832 + 0.5, acos(rd.y) / 3.1416)));
	
	// float sun = max(0.0, dot(rd, light));
	// float sky = max(0.0, dot(rd, vec3(0.0, 1.0, 1.0)));
	// float ground = max(0.0, -dot(rd, vec3(0.0, 1.0, 0.0)));
	// return  
	// 	(pow(sun, 256.0)+0.35*pow(sun, 2.0))*vec3(2.0, 1.6, 1.0) +
	// 	pow(ground, 0.5)*vec3(0.4, 0.3, 0.2)+pow(sky, 1.0)*vec3(0.5, 0.6, 0.7);
}


int indirections = 2;

vec2 uv = -1.0 + 2.0 * gl_FragCoord.xy / (iResolution.xy);
vec3 currentPos = (invView * vec4(0,0,0,1)).xyz;
vec3 initialPos=currentPos;

//vec3 currentDirOffset = normalize(currentPos + (invViewProjection * vec4(0, 0, 0.05+zoom, 0.0)).xyz);

// vec3 currentDir = normalize((invViewProjection * vec4(uv, 0.5, 0.0)).xyz);
vec2 fragCoord = vec2(gl_FragCoord.xy / iResolution.xy) * 2 - 1;
vec3 currentDir = normalize(inverse(mat3(projection) * mat3(view)) * vec3(fragCoord, 1));


vec3 initialDir = currentDir;

//float lengthCurrentDirOffset = length(currentDirOffset);
//float lengthUv = length(uv);
//float lengthExtraDepth = sqrt(lengthCurrentDirOffset*lengthCurrentDirOffset + lengthUv*lengthUv);
//float extraDepth = abs(lengthCurrentDirOffset-lengthExtraDepth);

void main(void) { 

normal = vec3(0.0);
initialDirNotnorm = vec4(invViewProjection * vec4(uv, 0.05, 0.0)).xyz;

	for (int i = 0; i <= indirections; i++) {
		currentDepth = 999999;
		int hitSphere = -1;
		int hitTriangle = -1;
		currentNormal = vec3(0,0,0);

		//============================//
		// get the closest hit object //
		//============================//

		// determine if it is a sphere

		for (int s = 0; s < sphereVec.length(); s++) {
			if (s != hitSphere) {			
				float hitDepth = sphere(currentPos, currentDir, vec3(sphereVec[s].xyz), sphereVec[s].w);
				if (hitDepth > 0.0 && hitDepth < currentDepth) {
					hitSphere = s;
					currentDepth = hitDepth;
				}
			}
		}

		// determine if it is a triangle
 
		for (int t = 0; t < myMesh.pos.length() && t != hitTriangle; t+=3) {
		  		float hitDepth = triangle(currentPos, normalize(currentDir), myMesh.pos[t].xyz, myMesh.pos[t+1].xyz, myMesh.pos[t+2].xyz);
		  	if (hitDepth < currentDepth && hitDepth>0.0) {
		  		hitSphere = -1;
		  		hitTriangle = t;
		  		currentDepth = hitDepth;
		  	}
		}

		//============================//
		// compute new ray parameters //
		//============================//

		// in case it is a sphere

		if (hitSphere >= 0) {
			// multiply Colors
			currentColor *= colorSphere[hitSphere];

			// make new Ray
			currentPos = currentPos + currentDir * currentDepth ;
			currentNormal = normalize(currentPos - sphereVec[hitSphere].xyz);
			currentDir = normalize(reflect(normalize(currentDir), currentNormal));
		}
		
		// in case it is a triangle

		 if (hitTriangle >= 0) {	
			// multiply Colors
			// currentColor *= currentPos;
			//currentColor *= abs(currentNormal);
			 currentColor *= vec3(0.8,0.4,0.2);
			// currentColor *=colorTriangle[hitTriangle/3];
			
			
			currentPos = currentPos + currentDir * currentDepth;
			mat3 a = mat3(vec3(myMesh.pos[hitTriangle].xyz), vec3(myMesh.pos[hitTriangle+1].xyz), vec3(myMesh.pos[hitTriangle+2].xyz));
			vec3 x = inverse(a) * (currentPos);
			currentNormal = normalize((myNormals.posNorm[hitTriangle].xyz * x.x) + (myNormals.posNorm[hitTriangle+1].xyz * x.y) + (myNormals.posNorm[hitTriangle+2].xyz * x.z));
			currentDirNotnorm = reflect(normalize(currentDir), currentNormal);
		 	currentDir = normalize(reflect(normalize(currentDir), currentNormal));
		 }
		
		if(i == 0){
			if (hitTriangle == -1 && hitSphere == -1) {
				diffuseColor = vec4(background(currentDir),1);
				diffusePosition = vec4(currentDir,0);
				diffuseDepth = vec3(999);
				reflectiveColor = vec4(0,0,0,0);
				reflectivePosition = vec4(0,0,0,0);
				reflectiveDepth = vec4(9999);
				break;
			} else {
				float phongDiffuse;
				// compute interpolated normal for triangle
				if(hitTriangle >= 0){
					mat3 a = mat3(vec3(myMesh.pos[hitTriangle].xyz), vec3(myMesh.pos[hitTriangle+1].xyz), vec3(myMesh.pos[hitTriangle+2].xyz));
					vec3 x = inverse(a) * currentPos;
					vec3 nor = normalize((myNormals.posNorm[hitTriangle].xyz * x.x) + (myNormals.posNorm[hitTriangle+1].xyz * x.y) + (myNormals.posNorm[hitTriangle+2].xyz * x.z));
					phongDiffuse = max(dot(nor, light),0) * 0.3;
					normal = nor;
				
				}
				else{
					phongDiffuse = max(dot(currentNormal, light),0) * 0.3;
					normal = currentNormal;
				}
				vec3  phongAmbient = vec3(0.0, 0.02, 0.01)*0.3;
				diffuseColor = vec4(currentColor * phongDiffuse + phongAmbient,1);
				diffusePosition = vec4(vec3(currentPos),1);
				
				//float temps = dot(currentDirNotnorm, initialDirNotnorm);
				//diffuseDepth = vec4(temps,temps,temps,1);
				vec3 depthVec = diffusePosition.xyz - initialPos;
				diffuseDepth = depthVec; //vec4(distance(initialPos, diffusePosition.xyz));
			}
		} 
		if (i == 1) {
			if (hitTriangle == -1 && hitSphere == -1) {		
				//'infinite' depth
				reflectivePosition= vec4(currentPos + currentDir * 1000,1);
				reflectiveDepth = vec4(1000);
			} else {
				reflectivePosition= vec4(vec3(currentPos),1);
				reflectiveDepth = vec4(vec3(currentDepth),1);
			}
		}
		if (i > 0) {
			currentColor *= background(currentDir);
			reflectiveColor = vec4(currentColor,1);
		}
	}
}