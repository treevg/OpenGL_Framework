#version 430 

in vec4 gl_FragCoord;

uniform vec3	iResolution; 
uniform float	iGlobalTime;	
uniform mat4	projection;
uniform float 	zoom;
uniform int		indirection;

uniform mat4 	invView;
uniform mat4	invViewProjection;

uniform vec4 	sphereVec[3];
uniform vec3 	mesh[3];
//uniform vec3 	colorSphere[3];

in 		vec4 	passPosition;

out 	vec4	fragColor;
out 	vec4 	fragPosition;

vec2 			closestHit=vec2(100.0,0.0);  //meaning: closestHit.x == value .y==0 hitPoint of a sphere
float 			mint;
int 			currentGeom;

vec3 			triangleNml;


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
	triangleNml=n;

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

vec3 refSphere(vec3 rd, int geomBase, int refDepth){
	vec3 color=vec3(0.1);
	vec3 nml2=vec3(0.0);
	int sphereHit;
	bool hasChanged=false;
	//vec3 tempColor=vec3(0.0);
	//float tempHit=100.0;

	if(refDepth==0){
		mint=100.0;
		return color;
	}
	for(int a=1; a<refDepth+1;a++){
		for(int i=0;i<sphereVec.length;i++){
		
			if(geomBase==i){continue;}
				
			//hittest from intersected point
			float t2= sphere(vec3(sphereVec[geomBase].x,sphereVec[geomBase].y,sphereVec[geomBase].z), rd, vec3(sphereVec[i].x,sphereVec[i].y,sphereVec[i].z),sphereVec[i].w);
				
			if(t2>0 && t2<mint){
				hasChanged=true;
				mint=t2;
				sphereHit=i;
				//vec3 nml2 = normalize(vec3(sphereVec[i].x,sphereVec[i].y,sphereVec[i].z) - (ro+rd*t2));
				//nml2 = normalize(vec3(sphereVec[i].x,sphereVec[i].y,sphereVec[i].z) - (vec3(sphereVec[geomBase].x,sphereVec[geomBase].y,sphereVec[geomBase].z)+rd*t2));
				nml2 = normalize((vec3(sphereVec[geomBase].x,sphereVec[geomBase].y,sphereVec[geomBase].z)+rd*t2) - vec3(sphereVec[sphereHit].x,sphereVec[sphereHit].y,sphereVec[sphereHit].z) );
				color =  vec3(t2);
			}	
		}
		
		if(hasChanged){
		
		// troublemaker here
		geomBase=sphereHit;
		
		rd=reflect(rd,nml2);
		hasChanged=false;
		}
	}	
	return color;
}


void hit(vec3 ro, vec3 rd){	

	float hitSphere, hitTriangle;

	for(int i=0; i<sphereVec.length();i++){
		hitSphere = sphere(ro, rd, vec3(sphereVec[i].x,sphereVec[i].y,sphereVec[i].z), sphereVec[i].w);
	
		if(hitSphere>0 && hitSphere<closestHit.x){
			closestHit=vec2(hitSphere,0.0);
			currentGeom=i;
		}
	}
	
//TODO make loop for generic meshsize
	//for(int i=0; i<mesh.length();i++){
	
		 hitTriangle = triangle(ro,rd,mesh);
		//hitTriangle=100.0; // placeholder
	
		if(hitTriangle>0 && hitTriangle<closestHit.x){
			closestHit=vec2(hitTriangle,1.0);
			//currentGeom=i;
		}
	//}
}


void draw(vec3 ro, vec3 rd){

	mint=100.0;	
	
	//hittest with every geometry
	hit(ro,rd);
		
	if(closestHit.x > 50){
		return;
	}
//TODO make drawSphere method

	//sphere was hit
	if(closestHit.y==0.0){
	
		// normal of intersected point of sphere
		vec3 nml = normalize(vec3(sphereVec[currentGeom].x,sphereVec[currentGeom].y,sphereVec[currentGeom].z) - (ro+rd*closestHit.x));
		
		//get reflectionvector of intersected spherepoint
		rd = reflect(rd, nml);

		//vec3 col = vec3(colorSphere[currentGeom].x,colorSphere[currentGeom].y,colorSphere[currentGeom].z);
		
		// compute indirection
		vec3 color = refSphere(rd,currentGeom,indirection);
			
		if(mint==100.0){
			
		} 
		else {
			// draws reflected point 
			//todo: fix normals  , choose gewichtungsfaktor correctly
			fragColor = vec4( color, 1.0 );
		}
		
	}
	
	//triangle was hit
	else{
	rd = reflect(rd, triangleNml);
	fragColor = vec4( closestHit.x,closestHit.x,closestHit.x, 1.0 );	
	
	}	 
}

void main(void)
{
	//vec4 pos = invView * vec4(0,0,0,1);
	//vec4 dir = normalize(invView * vec4(0,0,1,0));

	vec2 uv = -1.0 + 2.0 * gl_FragCoord.xy / iResolution.xy;
	vec3 ro = (invView * vec4(0,0,0,1)).xyz;
	vec3 rd = normalize((invViewProjection * vec4(uv, 0.04+zoom, 0.0)).xyz);

	
	fragColor=vec4(0.1);
	fragPosition = passPosition;

	draw(ro, rd);
}