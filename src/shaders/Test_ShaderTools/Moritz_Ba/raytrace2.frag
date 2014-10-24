#version 430 

in vec4 gl_FragCoord;

uniform vec3 iResolution; 	//viewport resolution in pixels
uniform float iGlobalTime;	//shader playback time in seconds
uniform vec4 sphere1;
uniform vector<vec4> spheres;


out vec4 fragColor;
out vec4 fragPosition;



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
 float sky = max(0.0, dot(rd, vec3(0.0, 1.0, 0.0)));
 float ground = max(0.0, -dot(rd, vec3(0.0, 1.0, 0.0)));
 return 
  (pow(sun, 256.0)+0.2*pow(sun, 2.0))*vec3(2.0, 1.6, 1.0) +
  pow(ground, 0.5)*vec3(0.4, 0.3, 0.2) +
  pow(sky, 1.0)*vec3(0.5, 0.6, 0.7);
}

void main(void)
{
 vec2 uv = (-1.0 + 2.0*gl_FragCoord.xy / iResolution.xy) * 
  vec2(iResolution.x/iResolution.y, 1.0);
 vec3 ro = vec3(0.0, 0.0, -3.0);
 vec3 rd = normalize(vec3(uv, 1.0));

for(auto sphere:spheres){
}
 
 float t = sphere(ro, rd, vec3(sphere1.x, sphere1.y, sphere1.z), sphere1.w);


 //vec3 p = vec3(0.0, 0.0, 0.0);
 //float t = sphere(ro, rd, p, 0.5); original

 vec3 nml = normalize(vec3(sphere1.x, sphere1.y, sphere1.z) - (ro+rd*t));
 vec3 bgCol = background(iGlobalTime, rd);
 rd = reflect(rd, nml);
 vec3 col = background(iGlobalTime, rd) * vec3(0.9, 0.8, 1.0);
 gl_FragColor = vec4( mix(bgCol, col, step(0.0, t)), 1.0 );
}


