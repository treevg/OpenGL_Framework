#version 430 

in vec4 gl_FragCoord;

uniform vec3	iResolution; 	//viewport resolution in pixels
uniform float	iGlobalTime;	//shader playback time in seconds	
uniform float 	side;
uniform float 	vertical;
uniform vec3 	mouse;
uniform mat4 	view;
uniform mat4	projection;

mat4 invView = inverse(view);
mat4 invViewProjection = inverse(projection * view);

uniform vec4 	sphereVec[3];
uniform vec3 	mesh[20];
uniform vec3 	colorSphere[3];

out vec4 	fragColor;
out vec4 	fragPosition;



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


void drawSphere(vec3 bgCol,vec3 ro, vec3 rd, vec2 uv){

	for(int i=0; i<sphereVec.length();i++){

		float t2=-1;
		float mint=1000.0;
		vec3 refColor;
		
		rd = normalize(vec3(uv, 1.0));
		//rd = normalize(invView * vec4(uv, 1.0, 0.0)).xyz;

		float t = sphere(ro, rd, vec3(sphereVec[i].x,sphereVec[i].y,sphereVec[i].z), sphereVec[i].w);

		if(t==-1 && gl_FragColor.xyz==bgCol.xyz){
			continue;
		}
	
		// normal of intersected point of sphere
		vec3 nml = normalize(vec3(sphereVec[i].x,sphereVec[i].y,sphereVec[i].z) - (ro+rd*t));

		//get reflectionvector of intersected spherepoint
		rd = reflect(rd, nml);

		//original color:0.9, 0.8, 1.0
		vec3 col = background(iGlobalTime, rd) * vec3(colorSphere[i].x,colorSphere[i].y,colorSphere[i].z);

		vec3 nml2=vec3(0.0);
		
		for(int j=0; j<sphereVec.length(); j++){
			
			if(j==i){continue;}
			//hittest from intersected point
			//needs: wo kommt strahl her(punkt), richtungsvektor(!), zu testende kugel 
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
	
	// hittest polygon: ebenengleichung	
//aus wie vielen wird das poly gezeichnet? dreiecke?


	float triangle(vec3 orig, vec3 dir, vec3 vertices[3], float lastHitT)
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
        return -1;
    t = (uv * wu - uu * wv) / D;
    if (t < 0.0 || (s + t) > 1.0)
        return -1.0;

    return (r > 1e-5) ? r : -1.0;
}
	
	
void drawPolygon(vec3 bgCol, vec3 ro, vec3 rd, vec2 uv){

for(int i=0; i<mesh.length();i++){

//float t=polygon(ro,rd,mesh[i]);
//if(gl_FragColor.xyz!=bgCol.xyz || t==-1){continue;}

}
}

void main2(void)
{


	// vec3 ro=vec3(mouse.x,mouse.y,mouse.z);  //vec3(0.0,0.0,-3.0);  
	// vec3 camdir=normalize(-ro);
	// vec3 camup= normalize(vec3(0.0,1.0,0.0));
	// vec3 camright = cross(camdir, camup);
	// camup = cross(camright, camdir);

	//uv-=ro.xy;	//kugelsicht! jedoch starr
	
	// vec3 rd = normalize(vec3(uv, 1.0));

	// float nori = (gl_FragCoord.x / 1280) - 0.5;
	// float norj = (gl_FragCoord.y / 720) - 0.5;
	// vec3 campoint=normalize(vec3(mouse.x,mouse.y,mouse.z));
	
	
	
	
	

	//vec3 r = vec3(nori*camright + norj*camup + campoint+camdir);
	 //rd = vec3(nori*camright, norj*camup , campoint+camdir);
	
	//vec3 raydir= r-campoint;
	
	vec4 pos = invView * vec4(0,0,0,1);
	vec4 dir = normalize(invView * vec4(0,0,1,0));


	vec2 uv = (-1.0 + 2.0*gl_FragCoord.xy / iResolution.xy) * 
		vec2(iResolution.x/iResolution.y, 1.0);


	vec3 bgCol = background(iGlobalTime, dir.xyz);

	gl_FragColor=vec4(bgCol,1.0);

	drawSphere(bgCol, pos.xyz, dir.xyz, uv);
	
	// drawPolygon(bgCol,ro,rd,uv);
}

void main(void)
{
	vec2 uv = -1.0 + 2.0 * gl_FragCoord.xy / iResolution.xy;
	vec3 ro = (invView * vec4(0,0,0,1)).xyz;
	vec3 rd = normalize((invViewProjection * vec4(uv, 0.05, 0.0)).xyz);

	vec3 p = vec3(0.0, 0.0, 0.0);
	float t = sphere(ro, rd, p, 1.0);
	vec3 nml = normalize(p - (ro+rd*t));
	vec3 bgCol = background(iGlobalTime, rd);
	rd = reflect(rd, nml);
	vec3 col = background(iGlobalTime, rd) * vec3(0.9, 0.8, 1.0);
	gl_FragColor = vec4( mix(bgCol, col, step(0.0, t)), 1.0 );
}