#version 430
// http://www.geeks3d.com/20100909/shader-library-gaussian-blur-post-processing-filter-in-glsl/
// edited by moe11elf

in vec2 passPosition;

uniform sampler2D tex; 
uniform vec2 resolution;

out vec4 fragColor;

float offset[3] = float[]( 0.0, 1.3846153846, 3.2307692308 );
float weight[3] = float[]( 0.2270270270, 0.3162162162, 0.0702702703 );

void main() 
{ 
  vec2 uv = (passPosition.xy-1)*0.5;
  vec3 vertRight;
  vec3 vertLeft;
  vec3 horizRight;
  vec3 horizLeft;
  vec3 tc = texture(tex, uv).rgb * weight[0] * weight[0];
  vec3 bl = tc;
  for (int i=1; i<3; i++) {
	vertRight = texture(tex, uv + vec2(0.0, offset[i])/resolution.y).rgb;
	if(vertRight != vec3(0)){
		tc += vertRight * weight[i];
	}
	vertLeft = texture(tex, uv - vec2(0.0, offset[i])/resolution.y).rgb;
	if(vertLeft != vec3(0)){
		tc += vertLeft * weight[i];
	}
	
  }
  for (int i=1; i<3; i++) {
	horizRight = texture(tex, uv + vec2(offset[i])/resolution.x, 0.0).rgb;
	if(horizRight != vec3(0)){
		tc += horizRight * weight[i];
	}
	horizLeft = texture(tex, uv - vec2(offset[i])/resolution.x, 0.0).rgb;
    if(horizLeft != vec3(0)){
		tc += horizLeft * weight[i];
	}
    }
  fragColor = vec4(bl, 1.0);
  //fragColor = vec4(max(tc,0.0), 1.0);
}