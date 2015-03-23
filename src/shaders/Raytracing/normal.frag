#version 430
in vec2 passPosition;

uniform sampler2D tex; 
uniform vec2 resolution;
uniform mat4 view;
uniform float factor;

out vec4 fragColor;

float offset[3] = float[]( 0.0, 1.3846153846, 3.2307692308 );
float weight[3] = float[]( 0.2270270270, 0.3162162162, 0.0702702703 );
vec3 eyeNew = vec4(inverse(view) * vec4(0,0,0,1)).xyz;
	vec3 negativeViewdirection = vec3(passPosition, 0.0) - eyeNew;


float eps = 0.2;

void main() 
{ 
  vec2 uv = (passPosition.xy - 1) * 0.5;
  vec3 tc = texture(tex, uv).xyz; // * weight[0] * weight[0];
  float n =  3.2307692308/ resolution.y;
  float n2 = 3.2307692308 / resolution.x;
  vec3 up = texture(tex, uv + vec2(0.0, n)).xyz;
  vec3 down =  texture(tex, uv - vec2(0.0, n)).xyz;
  vec3 right = texture(tex, uv + vec2(n, 0.0)).xyz;
  vec3 left =  texture(tex, uv - vec2(n, 0.0)).xyz;
  vec3 lengUp = abs(tc - up);
  vec3 lengDown = abs(tc - down);
  
  if(tc == vec3(0)){
	fragColor = vec4(0,0,0,1);
  }
  else{
  //detect edges
  if(length(lengUp) < eps){
	fragColor = vec4(tc, 1.0);
  }
  else{
	vec3 temp;
	//neighbour is background
	if(up == vec3(0)){
		temp = normalize(tc*(1-factor) + negativeViewdirection*factor );
	}
	//neighbour is another normal
	else{
		temp = normalize(tc*factor + negativeViewdirection*(1-factor));
	}
	fragColor = vec4(temp, 1.0);
  }
  }
  
 
}