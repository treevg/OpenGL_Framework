#version 430


uniform sampler2D color;
uniform sampler2D indirectionColor;
uniform sampler2D depth;
uniform sampler2D indirectionDepth;
uniform sampler2D fragPos;
uniform sampler2D fragPos2;

uniform int texNum;
uniform float minRange;
uniform float maxRange;
in vec4 passPosition;

out vec4 fragColor;


float LinearizeDepth(vec2 uv)
{
  float n = 1.0; // camera z near
  float f = 10.0; // camera z far
  float z = texture2D(depth, uv).x;
  return (2.0 * n) / (f + n - z * (f - n));	
}

vec3 lin() {
	float min= 1.0+minRange;
	float max = 20.0+maxRange;
	vec3 c = texture(depth, passPosition.xy).xyz;
	c = (c - min) / (max - min);
	
	return c;
    
}



void main() {
 
 
	if(texNum==0){
		fragColor = texture(color, passPosition.xy);
	
	}
 
	if(texNum==1){
		fragColor = texture(indirectionColor, passPosition.xy);
	}

	if(texNum==2){
		vec2 uv = passPosition.xy;
		float d = LinearizeDepth(uv);
		fragColor = vec4(lin(),1);
		
		//fragColor = vec4(d, d, d, 1.0);
		
		//fragColor = texture(depth, passPosition.xy);
	}	
	if(texNum==3){
		fragColor = texture(indirectionDepth, passPosition.xy);
	}	

}