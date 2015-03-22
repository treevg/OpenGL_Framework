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
  vec3 tc = vec3(1.0, 0.0, 0.0);
  vec2 uv = (passPosition.xy-1)*0.5;
  tc = texture(tex, uv).rgb * weight[0];
  for (int i=1; i<3; i++) {
	tc += texture(tex, uv + vec2(0.0, offset[i])/resolution.y).rgb * weight[i];
	tc += texture(tex, uv - vec2(0.0, offset[i])/resolution.y).rgb * weight[i];
  }
  for (int i=1; i<3; i++) {
    tc += texture(tex, uv + vec2(offset[i])/resolution.x, 0.0).rgb * weight[i];
    tc += texture(tex, uv - vec2(offset[i])/resolution.x, 0.0).rgb * weight[i];
    }
	
  fragColor = vec4(max(tc-0.75,0.0), 1.0);
}