#version 430 

// basics taken from: https://www.shadertoy.com/view/ldS3DW
// triangle intersection taken from: http://undernones.blogspot.de/2010/12/gpu-ray-tracing-with-glsl.html
// edited by moe11elf

in vec4 gl_FragCoord;

uniform vec3	resolution; 	
uniform mat4	projection;
uniform mat4 	invView;
uniform mat4 	view;
uniform vec4 	sphereVec[3];

in 		vec4	passPosition;
out 	vec3	normal;

layout(std430, binding=11) buffer meshData{
	vec4 pos[4200];		//4200
} myMesh;

layout(std430, binding=12) buffer meshNormal{
	vec4 posNorm[4200];  //4200
} myNormals;

float sphere(vec3 ray, vec3 dir, vec3 center, float radius) {
	vec3 rc = ray-center;
	float c = dot(rc, rc) - (radius*radius);
	float b = dot(dir, rc);
	float d = b*b - c;
	float t = -b - sqrt(abs(d));
	float st = step(0.0, min(t,d));
	return mix(-1.0, t, st);
}

float triangle(vec3 orig, vec3 dir, vec3 vertex0, vec3 vertex1, vec3 vertex2) {
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

vec3 currentPos = (invView * vec4(0,0,0,1)).xyz;
vec2 fragCoord = vec2(gl_FragCoord.xy / resolution.xy) * 2 - 1;
vec3 currentDir = normalize(inverse(mat3(projection) * mat3(view)) * vec3(fragCoord, 1));

void main(void) { 

	normal = vec3(0.0);

	float currentDepth = 999999;
	int hitSphere = -1;
	int hitTriangle = -1;

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

	// in case it is a sphere
	if (hitSphere >= 0) {
		// make new Ray
		currentPos = currentPos + currentDir * currentDepth ;
		normal = normalize(currentPos - sphereVec[hitSphere].xyz);
	}

	// in case it is a triangle
	 if (hitTriangle >= 0) {	
		currentPos = currentPos + currentDir * currentDepth;
		mat3 a = mat3(vec3(myMesh.pos[hitTriangle].xyz), vec3(myMesh.pos[hitTriangle+1].xyz), vec3(myMesh.pos[hitTriangle+2].xyz));
		vec3 x = inverse(a) * (currentPos);
		normal = normalize((myNormals.posNorm[hitTriangle].xyz * x.x) + (myNormals.posNorm[hitTriangle+1].xyz * x.y) + (myNormals.posNorm[hitTriangle+2].xyz * x.z));
	}
}