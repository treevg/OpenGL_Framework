#version 430


uniform sampler2D color;
uniform sampler2D indirectionColor;
uniform sampler2D depth;
uniform sampler2D indirectionDepth;
uniform int texNum;

in vec4 passPosition;

out vec4 fragColor;


float LinearizeDepth(vec2 uv)
{
  float n = 1.0; // camera z near
  float f = 100.0; // camera z far
  float z = texture2D(depth, uv).x;
  return (2.0 * n) / (f + n - z * (f - n));	
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
		fragColor = vec4(d, d, d, 1.0);
		
		//fragColor = texture(depth, passPosition.xy);
	}	
	if(texNum==3){
		fragColor = texture(indirectionDepth, passPosition.xy);
	}	

}