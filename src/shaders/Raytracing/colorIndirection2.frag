#version 430 

in vec4 gl_FragCoord;

uniform vec3	iResolution; 	
uniform mat4	projection;
uniform float 	zoom;
uniform int		indirection;

uniform mat4 	invView;
uniform mat4	invViewProjection;

uniform vec4 	sphereVec[3];
uniform vec3 	mesh[3];
uniform vec3 	colorSphere[3];

out 	vec4	fragColor;
out 	vec4 	fragPosition;


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

float triangle(vec3 orig, vec3 dir, vec3 vertices[3])
{
   // const float INFINITY = 1e10;
    vec3 u, v, n; // triangle vectors
    vec3 w0, w;  // ray vectors
    float r, a, b; // params to calc ray-plane intersect

    // get triangle edge vectors and plane normal
    u = vertices[1] - vertices[0];
    v = vertices[2] - vertices[0];
    n = cross(u, v);

    w0 = orig - vertices[0];
    a = -dot(n, w0);
    b = dot(n, dir);
    if (abs(b) < 1e-5)
    {
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
    w = I - vertices[0];
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

vec3 background(float t, vec3 rd)
{
	vec3 light = normalize(vec3(sin(t), 0.6, cos(t)));
	float sun = max(0.0, dot(rd, light));
	float sky = max(0.0, dot(rd, vec3(0.0, 1.0, 1.0)));
	float ground = max(0.0, -dot(rd, vec3(0.0, 1.0, 0.0)));
	return  
		(pow(sun, 256.0)+0.35*pow(sun, 2.0))*vec3(2.0, 1.6, 1.0) +
		pow(ground, 0.5)*vec3(0.4, 0.3, 0.2)+pow(sky, 1.0)*vec3(0.5, 0.6, 0.7);
}






int indirections = 2;

vec2 uv = -1.0 + 2.0 * gl_FragCoord.xy / iResolution.xy;
vec3 currentPos = (invView * vec4(0,0,0,1)).xyz;
vec3 currentDir = normalize((invViewProjection * vec4(uv, 0.04+zoom, 0.0)).xyz);
vec3 currentColor = vec3(1,1,1);
float currentDepth;
vec3 currentNormal;

int timesReflected=0;
vec3 temp = vec3(1.0);
vec3 saveDir=currentDir;
vec3 savePos=currentPos;

void main(void)
{
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

		// for (int t = 0; t < mesh.length() && t != hitTriangle; t+=3) {
		// 	float hitDepth = 
		// 	if (hitDepth < currentDepth) {
		// 		hitSphere = -1;
		// 		hitTriangle = t;
		// 		currentDepth = hitDepth;
		// 	}
		// }

		//============================//
		// compute new ray parameters //
		//============================//

		// in case it is a sphere

		if (hitSphere >= 0) {
			timesReflected++;
			// multiply Colors
			temp *= colorSphere[hitSphere];
			
			// make new Ray
			currentPos = currentPos + currentDir * currentDepth;
			currentNormal = normalize(currentPos - sphereVec[hitSphere].xyz);
			currentDir = normalize(reflect(normalize(currentDir), currentNormal));
		}
		
		// in case it is a triangle

		// if (hitTriangle >= 0) {	
		// 	currentPos = 
		// 	currentNormal = 
		// 	currentDir = 
		// }
	}
	
	
	if(timesReflected>1){
		currentColor=temp;
	}
	else{
		currentDepth=999999;
		currentDir= saveDir;
		currentPos=savePos;
	}

	// and finally the background color
	currentColor *= background(currentDepth, currentDir);
	fragColor = vec4(currentColor,1);
	fragPosition = vec4(vec3(currentPos),1);
}