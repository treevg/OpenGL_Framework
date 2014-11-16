#version 430 

in vec4 gl_FragCoord;

uniform vec3	iResolution; 	//viewport resolution in pixels
uniform float	iGlobalTime;	//shader playback time in seconds	
uniform mat4	projection;
uniform float 	zoom;

uniform mat4 	invView;
uniform mat4	invViewProjection;

uniform vec4 	sphereVec[3];
uniform vec3 	mesh[20];
uniform vec3 	colorSphere[3];

out vec4		fragColor;
out vec4 		fragPosition;

float 			closestHit=100;



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


float sphereRec(vec3 ray, vec3 dir, int geom)
{
	if(geom<sphereVec.length()){
	vec3 rc = ray- vec3(sphereVec[geom].x,sphereVec[geom].y,sphereVec[geom].z); 
	float c = dot(rc, rc) - (sphereVec[geom].w * sphereVec[geom].w);
	float b = dot(dir, rc);
	float d = b*b - c;
	float t = -b - sqrt(abs(d));
	float st = step(0.0, min(t,d));
	float lastHit = mix(-1.0, t, st);
	
	if(lastHit>0 && lastHit<closestHit){
	closestHit=lastHit;
	}
	return sphereRec(ray,dir,geom+1);
	}
	return closestHit;
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

// make draw method.  insert global lastHit
// globale var mit lasthit?
void drawSphere(vec3 bgCol,vec3 ro, vec3 rd, vec2 uv, int recDepth){

	vec3 saverd = normalize((invViewProjection * vec4(uv, 0.04+zoom, 0.0)).xyz);
	
	for(int i=0; i<sphereVec.length();i++){

		float t2=-1;
		float mint=1000.0;
		vec3 refColor;
		rd= saverd;

		float t = sphere(ro, rd, vec3(sphereVec[i].x,sphereVec[i].y,sphereVec[i].z), sphereVec[i].w);

		if(t==-1 && gl_FragColor.xyz==bgCol.xyz){
			continue;
		}
	
		// normal of intersected point of sphere
		vec3 nml = normalize(vec3(sphereVec[i].x,sphereVec[i].y,sphereVec[i].z) - (ro+rd*t));
		//get reflectionvector of intersected spherepoint
		rd = reflect(rd, nml);

		vec3 col = background(iGlobalTime, rd) * vec3(colorSphere[i].x,colorSphere[i].y,colorSphere[i].z);

		vec3 nml2=vec3(0.0);
		
		
		//make reflection method. mit for(i<recDepthloop) 
		
		for(int i=0; i<refDepth;i++){
		// initialisiere normalen etc immer neu
		vec3 color += refSphere();  //parametersuche
		// nml neu, geoms hochzählen


		}
		
		for(int j=0; j<sphereVec.length(); j++){
			
			if(j==i){continue;}
			//hittest from intersected point
			t2= sphere(vec3(sphereVec[i].x,sphereVec[i].y,sphereVec[i].z), rd, vec3(sphereVec[j].x,sphereVec[j].y,sphereVec[j].z),sphereVec[j].w);

			if(t2>0 && t2<mint){
				mint=t2;
				vec3 nml2 = normalize(vec3(sphereVec[j].x,sphereVec[j].y,sphereVec[j].z) - (ro+rd*t2));
				refColor = background(iGlobalTime, nml2)* vec3(colorSphere[j].x , colorSphere[j].y, colorSphere[j].z);
			}	
		}

		if(gl_FragColor.xyz==bgCol.xyz){

			if(mint==1000.0){
				gl_FragColor = vec4( mix(bgCol, col, step(0.0, t)), 1.0 )+0.05;	
			} else {
				// draws reflected point 
				//todo: fix normals  , choose gewichtungsfaktor correctly

				vec4 temp= vec4( mix(bgCol, col, step(0.0, t)), 1.0 );
				gl_FragColor = vec4( mix(vec3(temp.y,temp.y,temp.z), refColor, mint), 1.0 )+0.05;
				//gl_FragColor = vec4(mix(bgCol, vec3(temp.x,temp.y,temp.z), step(0.0,mint)),1.0);
			}
		}
	}  
}

vec3 refSphere(vec3 ro, vec3 rd, int refDepth, int geomBase, int geomRef){
	vec3 color=vec3(0.0);
	vec3 nml2=vec3(0.0);
	for(int i=0;i<sphereVec.length;i++){
		if(geomBase==geomRef){continue;}
			//hittest from intersected point
			float t2= sphere(vec3(sphereVec[geomBase].x,sphereVec[geomBase].y,sphereVec[geomBase].z), rd, vec3(sphereVec[geomRef].x,sphereVec[geomRef].y,sphereVec[geomRef].z),sphereVec[geomRef].w);

			if(t2>0 && t2<mint){
				mint=t2;
				vec3 nml2 = normalize(vec3(sphereVec[geomRef].x,sphereVec[geomRef].y,sphereVec[geomRef].z) - (ro+rd*t2));
				Color = background(iGlobalTime, nml2)* vec3(colorSphere[geomRef].x , colorSphere[geomRef].y, colorSphere[geomRef].z);
			}	
		}
	return refColor;
}

float draw(vec3 ro, vec3 rd){	
	float t = sphereRec(ro, rd, 0);	
return t;
}



void main(void)
{
	vec4 pos = invView * vec4(0,0,0,1);
	vec4 dir = normalize(invView * vec4(0,0,1,0));

	vec2 uv = -1.0 + 2.0 * gl_FragCoord.xy / iResolution.xy;
	vec3 ro = (invView * vec4(0,0,0,1)).xyz;
	vec3 rd = normalize((invViewProjection * vec4(uv, 0.04+zoom, 0.0)).xyz);

	vec3 bgCol = background(iGlobalTime, rd);

	gl_FragColor=vec4(bgCol,1.0);

	drawSphere(bgCol, ro, rd, uv,1);
}

void main2(void)
{
	vec2 uv = -1.0 + 2.0 * gl_FragCoord.xy / iResolution.xy;
	vec3 ro = (invView * vec4(0,0,0,1)).xyz;
	vec3 rd = normalize((invViewProjection * vec4(uv, 0.04+zoom, 0.0)).xyz);

	vec3 p = vec3(0.0, 0.0, 0.0);
	float t = sphere(ro, rd, p, 1.0);
	vec3 nml = normalize(p - (ro+rd*t));
	vec3 bgCol = background(iGlobalTime, rd);
	rd = reflect(rd, nml);
	vec3 col = background(iGlobalTime, rd) * vec3(0.9, 0.8, 1.0);
	gl_FragColor = vec4( mix(bgCol, col, step(0.0, t)), 1.0 );
}