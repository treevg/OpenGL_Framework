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
uniform vec3 	mesh[20];
uniform vec3 	colorSphere[3];

in 		vec4 	passPosition;

 out 	vec4	fragColor;
out 	vec4 	fragPosition;

vec2 			closestHit=vec2(100.0,0.0);  //meaning: closestHit.x == value .y==0 hitPoint of a sphere
float 			mint;
int 			currentGeom;


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

vec3 refSphere(vec3 rd, int geomBase, int refDepth){
	vec3 color=vec3(0.0);
	vec3 nml2=vec3(0.0);
	int sphereHit;
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
				mint=t2;
				sphereHit=i;
				//vec3 nml2 = normalize(vec3(sphereVec[i].x,sphereVec[i].y,sphereVec[i].z) - (ro+rd*t2));
				//nml2 = normalize(vec3(sphereVec[i].x,sphereVec[i].y,sphereVec[i].z) - (vec3(sphereVec[geomBase].x,sphereVec[geomBase].y,sphereVec[geomBase].z)+rd*t2));
				nml2 = normalize((vec3(sphereVec[geomBase].x,sphereVec[geomBase].y,sphereVec[geomBase].z)+rd*t2) - vec3(sphereVec[sphereHit].x,sphereVec[sphereHit].y,sphereVec[sphereHit].z) );
				color = background(iGlobalTime, nml2) * (vec3(colorSphere[sphereHit].x , colorSphere[sphereHit].y, colorSphere[sphereHit].z));
			}	
		}
		// troublemaker here
		geomBase=sphereHit;
		
		rd=reflect(rd,nml2);
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
	
	for(int i=0; i<mesh.length();i++){
	
		// hitTriangle = triangle();
		hitTriangle=100.0; // placeholder
	
		if(hitTriangle>0 && hitTriangle<closestHit.x){
			closestHit=vec2(hitTriangle,1.0);
			currentGeom=i;
		}
	}
}


void draw(vec3 bgCol,vec3 ro, vec3 rd){

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

		vec3 col = background(iGlobalTime, rd) * vec3(colorSphere[currentGeom].x,colorSphere[currentGeom].y,colorSphere[currentGeom].z);
		
		// gets reflection color
		vec3 color = refSphere(rd,currentGeom,indirection);
			
		if(mint==100.0){
			fragColor = vec4( mix(bgCol, col, step(0.0, closestHit.x)), 1.0 )+0.05;	
		} 
		else {
			// draws reflected point 
			//todo: fix normals  , choose gewichtungsfaktor correctly
			vec4 temp= vec4( mix(bgCol, col, step(0.0, closestHit.x)), 1.0 );
			fragColor = vec4( mix(vec3(temp.y,temp.y,temp.z), color, mint), 1.0 )+0.05;
		}
		
	}
	//triangle was hit
	else{}	 
}

void main(void)
{
 
	//vec4 pos = invView * vec4(0,0,0,1);
	//vec4 dir = normalize(invView * vec4(0,0,1,0));

	vec2 uv = -1.0 + 2.0 * gl_FragCoord.xy / iResolution.xy;
	vec3 ro = (invView * vec4(0,0,0,1)).xyz;
	vec3 rd = normalize((invViewProjection * vec4(uv, 0.04+zoom, 0.0)).xyz);

	vec3 bgCol = background(iGlobalTime, rd);
	fragColor=vec4(bgCol,1.0);
fragPosition = passPosition;
	draw(bgCol, ro, rd);
}